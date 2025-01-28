#include <Arduino.h>

// Pinos do sensor Front
#define FRONT_TRIG 26
#define FRONT_ECHO 27

// Variáveis globais
volatile unsigned long echo_start_time = 0;  // Marca o início do pulso Echo
volatile unsigned long echo_duration = 0;   // Duração do pulso Echo
volatile bool pulse_captured = false;       // Indica se o pulso foi capturado

// Velocidade do som (em cm/µs)
const float SOUND_SPEED = 0.0343;

// Interrupção para detectar mudanças no Echo
void IRAM_ATTR handle_echo_change() {
    if (digitalRead(FRONT_ECHO) == HIGH) {
        // Marca o início do pulso Echo
        echo_start_time = micros();
    } else if (digitalRead(FRONT_ECHO) == LOW && echo_start_time > 0) {
        // Calcula a duração do pulso Echo
        echo_duration = micros() - echo_start_time;
        pulse_captured = true; // Pulso completo foi capturado
    }
}

void setup() {
    // Configuração inicial
    Serial.begin(115200);

    // Configuração dos pinos
    pinMode(FRONT_TRIG, OUTPUT);
    pinMode(FRONT_ECHO, INPUT);

    // Configura a interrupção no pino Echo
    attachInterrupt(digitalPinToInterrupt(FRONT_ECHO), handle_echo_change, CHANGE);

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
    delay(100);
}
