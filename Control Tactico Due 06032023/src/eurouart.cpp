#include "eurouart.hpp"
#include <Arduino.h>
#include "motion_controller.hpp"

#include "protocol.hpp"

extern motion_controller controlador_tactico;

bool stringComplete = false;

void (*reset_arduino)(void) = 0;

// Protocol variables
uahruart::parser::Protocol protocol;


void setup_serial() {
    protocol.on_write([](const char* msg) {
        Serial.println(msg);
    });

    pinMode(LED_BUILTIN, OUTPUT);

    // Add calls
    protocol.register_method("traction", "turn", {[](int32_t arg) {
        controlador_tactico.ref_ang = static_cast<float>(arg);
        controlador_tactico.prev_move_calculus(0);
    }});

    protocol.register_method("traction", "advance", {[](int32_t arg) {
        controlador_tactico.ref_distancia = static_cast<float>(arg);
        controlador_tactico.prev_move_calculus(1);
    }});
}

void serialEvent()
{
    static string inputString = "";
    while(Serial.available() > 0)
    {
        inputString = string(Serial.readStringUntil('\n').c_str());

        if(inputString!="")
        {
          // Traduccion_Variables();
            protocol.receive(inputString);
            return;
        }
    }
}

void Traduccion_Variables()       
{ //Se encarga de leer el mensaje recibido, actualizar las variables y levantar los flags.
  // char Instruccion_Codigo = inputString.charAt(0);
  // char Signo = inputString.charAt(1);
  // float grados_giro = 0;
  // float distancia = 0;
  // // Serial.println(Instruccion_Codigo);
  //   switch (Instruccion_Codigo)
  //   {
  //   case ('G'):
  //       grados_giro = (inputString.charAt(2) - '0') * 100 + (inputString.charAt(3) - '0') * 10 + (inputString.charAt(4) - '0');
  //       if(Signo == '-')
  //         grados_giro = -grados_giro;       
  //       controlador_tactico.ref_ang = grados_giro;
  //       controlador_tactico.prev_move_calculus(0);
  //       break;
  //   case ('D'):
  //       distancia = (inputString.charAt(2) - '0') * 1000 + (inputString.charAt(3) - '0') * 100 + (inputString.charAt(4) - '0') * 10 + (inputString.charAt(5) - '0');
  //       Serial.println("Distancia recibida: "); Serial.println(distancia);
  //       if(inputString.charAt(1) == '-')
  //         distancia = -distancia;
  //       controlador_tactico.ref_distancia = distancia;
  //       controlador_tactico.prev_move_calculus(1);
  //       break;
  //   case ('R'):
  //       reset_arduino();
  //       break;
  //   default:
  //       Serial.println("EI" + inputString);
  //       Instruccion_Codigo = 1;
  //       break;
  //   }
  // inputString = "";
}
