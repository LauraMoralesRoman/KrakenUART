#include "eurouart.hpp"
#include <Arduino.h>
#include "motion_controller.hpp"
#include "protocol.hpp"

extern motion_controller controlador_enzima;

String inputString = "";
bool stringComplete = false;

void (*reset_arduino)(void) = 0;

uahruart::parser::Protocol protocol;

void setupProtocol() {
    protocol.on_write([](const char* msg) {
        Serial.print(msg);
        Serial.print("\n\0");
    });

    protocol.register_method("traction", "turn", [](int32_t arg) {
        controlador_enzima.ref_ang = static_cast<float>(arg);
        controlador_enzima.prev_move_calculus(0);
    });

    protocol.register_method("traction", "advance", [](int32_t arg) {
        controlador_enzima.ref_distancia = static_cast<float>(arg);
        controlador_enzima.prev_move_calculus(1);
    });

    on_finished([]() {
        uahruart::primitives::Bool finished = true;
        protocol.send(finished);
    });
}

void serialEvent()
{
  while(Serial.available()>0)
  {
    auto inputString = string(Serial.readStringUntil('\n').c_str());

    if(inputString!="")
    {
        protocol.receive(inputString);
      // Serial.println(inputString);
      // Traduccion_Variables();
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
  //       if(inputString.charAt(1) == '-')
  //         grados_giro = -grados_giro;       
  //       controlador_enzima.ref_ang = grados_giro;
  //       controlador_enzima.prev_move_calculus(0);
  //       break;
  //   case ('D'):
  //       distancia = (inputString.charAt(2) - '0') * 1000 + (inputString.charAt(3) - '0') * 100 + (inputString.charAt(4) - '0') * 10 + (inputString.charAt(5) - '0');
  //       Serial.println("Distancia recibida: "); Serial.println(distancia);
  //       if(inputString.charAt(1) == '-')
  //         distancia = -distancia;
  //       controlador_enzima.ref_distancia = distancia;
  //       controlador_enzima.prev_move_calculus(1);
  //       break;
		// case ('R'):
		// 		reset_arduino();
		// 		break;
  //   default:
  //       Serial.println("EI" + inputString);
  //       Instruccion_Codigo = 1;
  //       break;
  //   }
  // inputString = "";
}
