#include <Arduino.h>

// Pinos do sensor Front
#define FRONT_TRIG 26
#define FRONT_ECHO 27

// Variáveis globais
volatile unsigned long echo_start_time = 0;  // Marca o início do pulso Echo
volatile unsigned long echo_duration = 0;   // Duração do pulso Echo
volatile bool pulse_captured = false;       // Indica se o pulso foi capturado

// Velocidade do som (cm/µs)
const float SOUND_SPEED = 0.0343;

// Interrupção para detectar o início do pulso (RISING)
void IRAM_ATTR handle_echo_rise() {
    echo_start_time = micros(); // Captura o tempo de início do pulso
}

// Interrupção para detectar o final do pulso (FALLING)
void IRAM_ATTR handle_echo_fall() {
    if (echo_start_time > 0) {
        echo_duration = micros() - echo_start_time; // Calcula a duração
        pulse_captured = true;                     // Marca o pulso como capturado
        echo_start_time = 0;                       // Reseta para a próxima medição
    }
}

void setup() {
    Serial.begin(115200);

    // Configuração dos pinos
    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);

    // Configura as interrupções no pino Echo
    attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), handle_echo_rise, RISING);
    attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), handle_echo_fall, FALLING);

    Serial.println("Sistema iniciado. Monitorando o sensor Front...");
}

void loop() {
    // Envia o pulso Trigger
    digitalWrite(FRONT_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(FRONT_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(FRONT_TRIG, LOW);

    // Verifica se o pulso foi capturado
    if (pulse_captured) {
        // Calcula a distância em centímetros
        float distance = echo_duration * SOUND_SPEED / 2.0;

        // Exibe os resultados no Serial Monitor
        Serial.print("Pulso Echo (us): ");
        Serial.print(echo_duration);
        Serial.print(" | Distância (cm): ");
        Serial.println(distance);

        // Envia apenas a distância para o Serial Plotter
        Serial.println(distance);

        // Reseta a flag para a próxima medição
        pulse_captured = false;
    }

    // Pequeno intervalo antes da próxima medição
    delay(50);
}
