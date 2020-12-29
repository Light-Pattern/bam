
#include <Arduino.h>
const int pinButton = 23;
#define DEBOUNCETIME 10 //tempo máximo de debounce para o botão (ms)
//É DECLARADA VOLÁTIL PORQUE SERÁ COMPARTILHADA PELO ISR E PELO CÓDIGO PRINCIPAL
volatile int numberOfButtonInterrupts = 0; //número de vezes que a interrupção foi executada
volatile bool lastState; //guarda o último estado do botão quando ocorreu a interrupção
volatile uint32_t debounceTimeout = 0; //guarda o tempo de debounce
//variáveis para controle dentro do loop
uint32_t saveDebounceTimeout;
bool saveLastState;
int save;
// For setting up critical sections (enableinterrupts and disableinterrupts not available)
// used to disable and interrupt interrupts
// Para configurar seções críticas (interrupções de ativação e interrupções de desativação não disponíveis)
// usado para desabilitar e interromper interrupções
portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
// Interrupt Service Routine - Keep it short!
//Interrupção
//Interrompe a rotina de serviço
//IRAM_ATTR --> é utilizado para indicar que esse trecho de código ficará na seção do barramento de instruções da RAM (maior velocidade) e não na Flash 
//portENTER_CRITICAL_ISR /´portEXIT_CRITICAL_ISR --> Isso é necessário porque a variável que vamos usar também é alterada pelo loop principal, 
                                                    //como visto anteriormente, e precisamos evitar problemas de acesso simultâneo.
void IRAM_ATTR handleButtonInterrupt() {
    portENTER_CRITICAL_ISR(&mux); 
    
      numberOfButtonInterrupts++;
      lastState = digitalRead(pinButton);  
      debounceTimeout = xTaskGetTickCount(); //versão do millis () que funciona a partir da interrupção //version of millis() that works from interrupt
    portEXIT_CRITICAL_ISR(&mux);
}

void setup() {
  Serial.begin(9600);
pinMode(pinButton, INPUT);
attachInterrupt(digitalPinToInterrupt(pinButton), handleButtonInterrupt, CHANGE);
}

void loop()
{
    portENTER_CRITICAL_ISR(&mux); // início da seção crítica
      save  = numberOfButtonInterrupts;
      saveDebounceTimeout = debounceTimeout;
      saveLastState  = lastState;
    portEXIT_CRITICAL_ISR(&mux); // fim da seção crítica
 
    bool currentState = digitalRead(pinButton); //recupera o estado atual do botão
 
    //se o estado do botão mudou, atualiza o tempo de debounce
    if(currentState != saveLastState)
    {
      saveDebounceTimeout = millis();
    }
 
    //se o tempo passado foi maior que o configurado para o debounce e o número de interrupções ocorridas é maior que ZERO (ou seja, ocorreu alguma), realiza os procedimentos
    if( (millis() - saveDebounceTimeout) > DEBOUNCETIME && (save != 0) )
    {
           
            Serial.printf("Button Interrupt Triggered %d times, current State=%u, time since last trigger %dms\n", save, currentState, millis() - saveDebounceTimeout);
            portENTER_CRITICAL_ISR(&mux);  //início da seção crítica
              numberOfButtonInterrupts = 0; // reconhece que o botão foi pressionado e reseta o contador de interrupção //acknowledge keypress and reset interrupt counter
            portEXIT_CRITICAL_ISR(&mux); //fim da seção crítica
    }
}