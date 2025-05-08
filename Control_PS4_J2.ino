#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// Envía por UART1 y muestra en consola
void enviarUART(const char* mensajeTexto, char caracterUART) {
    Serial.printf("Se presionó %s y se envió %c\n", mensajeTexto, caracterUART);
    Serial1.println(caracterUART);  // ENVÍA dato + salto de línea
}

void onConnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                          ctl->getModelName().c_str(), properties.vendor_id,
                          properties.product_id);
            myControllers[i] = ctl;
            return;
        }
    }
    Serial.println("CALLBACK: Controller connected, but could not find empty slot");
}

void onDisconnectedController(ControllerPtr ctl) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            return;
        }
    }
}

void processGamepad(ControllerPtr ctl) {
    if (ctl->dpad() & DPAD_UP) {
        enviarUART("ARRIBA", 'u');
    }
    if (ctl->dpad() & DPAD_DOWN) {
        enviarUART("ABAJO", 'd');
    }
    if (ctl->dpad() & DPAD_LEFT) {
        enviarUART("IZQUIERDA", 'l');
    }
    if (ctl->dpad() & DPAD_RIGHT) {
        enviarUART("DERECHA", 'r');
    }

    if (ctl->a()) {
        enviarUART("X", 'x');
    }
    if (ctl->b()) {
        enviarUART("O", 'o');
    }
}

void processMouse(ControllerPtr ctl) {}
void processKeyboard(ControllerPtr ctl) {}
void processBalanceBoard(ControllerPtr ctl) {}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            }
        }
    }
}

void setup() {
    Serial.begin(115200); // Consola USB para debug

    // UART1: TX en GPIO 17 (envía hacia J1)
    Serial1.begin(9600, SERIAL_8N1, -1, 17); // RX no se usa (-1)

    Serial.println("Iniciando Bluepad32...");
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys(); // Puedes comentar esto si ya estaba emparejado
    BP32.enableVirtualDevice(false); // No queremos mouse

    delay(2000); // Tiempo para estabilizar Bluetooth
}

void loop() {
    bool dataUpdated = BP32.update();
    if (dataUpdated) {
        processControllers(); // Procesa si se presionó algo
    }

    delay(150); // Espera pequeña entre ciclos
}
