#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
String datosJ2 = "";

// Envía solo 1 carácter a través de TX (sin \n)
void enviarUART(const char* mensajeTexto, char caracterUART) {
    Serial.printf("Se presionó %s y se envió %c\n", mensajeTexto, caracterUART);
    Serial1.write(caracterUART);  // Solo el carácter, sin enter
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
            // Cambiar el color del LED a rojo
            ctl->setColorLED(255, 0, 0);
            
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
    static uint32_t prevButtons = 0;
    static uint8_t prevDpad = 0;

    uint32_t buttonsNow = ctl->buttons();
    uint8_t dpadNow = ctl->dpad();

    if ((dpadNow & DPAD_UP) && !(prevDpad & DPAD_UP))         enviarUART("ARRIBA", 'U');
    if ((dpadNow & DPAD_DOWN) && !(prevDpad & DPAD_DOWN))     enviarUART("ABAJO", 'D');
    if ((dpadNow & DPAD_LEFT) && !(prevDpad & DPAD_LEFT))     enviarUART("IZQUIERDA", 'L');
    if ((dpadNow & DPAD_RIGHT) && !(prevDpad & DPAD_RIGHT))   enviarUART("DERECHA", 'R');

    if ((buttonsNow & BUTTON_A) && !(prevButtons & BUTTON_A)) enviarUART("X", 'X');
    if ((buttonsNow & BUTTON_B) && !(prevButtons & BUTTON_B)) enviarUART("O", 'O');

    prevButtons = buttonsNow;
    prevDpad = dpadNow;
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
    Serial.begin(115200);

    Serial1.begin(9600, SERIAL_8N1, -1, 17);  // TX hacia Nucleo
    Serial2.begin(9600, SERIAL_8N1, 16, -1);  // RX desde J2

    Serial.println("Iniciando Bluepad32...");
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
    BP32.enableVirtualDevice(false);

    delay(2000);
}

void loop() {
    bool dataUpdated = BP32.update();

    while (Serial2.available()) {
        char c = Serial2.read();
        datosJ2 += c;

        if (c == '\n') {
            datosJ2.trim();  // Elimina \r y espacios

            if (datosJ2.length() == 1) {
                Serial.printf("Recibido de J2: %s\n", datosJ2.c_str());
                Serial1.write(datosJ2[0]);  // Solo el carácter, sin \n
            } else {
                Serial.println("Dato de J2 inválido. Ignorado.");
            }

            datosJ2 = "";
        }
    }

    if (dataUpdated) {
        processControllers();
    }

    delay(100);
}
