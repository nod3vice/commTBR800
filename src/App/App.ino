#include <MsTimer2.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <stdio.h>

#define ledstat 4

byte sb;
byte pkgRead[400];
byte pkgWrite[] = {0x05, 0x64, 0x0b, 0xc4, 0x01, 0x00, 0x01, 0x00, 0xc2, 0x2e, 0xc0, 0xc0, 0x01, 0x3c, 0x01, 0x06, 0xff, 0x50};

int index, tempopisca, tempoenvio = 0;

String Sigfox, msgGSM, pkgToSend, numHex0, numHex1, numHex2, numHex3, numHex4, numHex5,
    numHex6, numHex7, numHex8, numHex9, numHex10, numHex11, var1, var2, var3, valuesToChange = "";

bool initpack = false;
bool lersigfox, enviaGSM = false;

AltSoftSerial sigfoxSerial;
SoftwareSerial gsmSerial(11, 10);

void setup(){
    Serial.begin(9600);
    MsTimer2::set(1000, requestData); // interruption  600000
    MsTimer2::start();

    pinMode(ledstat, OUTPUT);
    sigfoxSerial.begin(9600);
    gsmSerial.begin(4800);
    

    sigfoxSerial.println("AT");
    delay(100);
    sigfoxSerial.println("AT$RC");
    Sigfox = "AT$SF=hello";
    delay(100);
    sigfoxSerial.println(Sigfox);
    delay(100);

    Serial.println("Initializing...");
    delay(5000);
    gsmSerial.println("AT+CREG?"); //Once the handshake test is successful, it will back to OK
    updateSerial();
    delay(2000);
    gsmSerial.println("AT+CMGF=1"); // Configuring TEXT mode
    updateSerial();
    delay(2000);
    gsmSerial.println("AT+CNMI=2,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
    updateSerial();
    delay(2000);

    initpack = true;
}

// Convert data to 16-bit integer
int convert16bits(char msb, char lsb){
    int convertData = (int(msb * 256) + (int(lsb)));
    return convertData;
}

// Interruption to receive serial data
void serialEvent(){
    while ( Serial.available() )    {
        sb = Serial.read();
        pkgRead[index] = sb;
        index++;
    }
    if ( index >= 246 ){
        sendToSigfox();
        index = 0;
    }
}

// Read the Voltage Regulator values
void requestData(){
    ++tempoenvio;
    ++tempopisca;
    if ( (tempoenvio == 420) || (initpack == true) ){
        lersigfox = true;
        initpack = false;
    }
}

// Show received packet parameters
void sendToSigfox(){
    delay(100);
    convertVariables();
    Sigfox = "";
    sigfoxSerial.println("AT");
    sigfoxSerial.println("AT$RC");
    Sigfox = "AT$SF=";
    delay(100);
    Serial.println("Enviando para nuvem : " + pkgToSend);
    Sigfox += pkgToSend;
    sigfoxSerial.println(Sigfox);
}

void convertVariables(){
    numHex0 = String((convert16bits(pkgRead[32], pkgRead[31])), HEX);
    numHex1 = String((convert16bits(pkgRead[35], pkgRead[34])), HEX);
    numHex2 = String((convert16bits(pkgRead[38], pkgRead[37])), HEX);
    numHex3 = String((convert16bits(pkgRead[41], pkgRead[40])), HEX);
    numHex4 = String((convert16bits(pkgRead[46], pkgRead[43])), HEX);
    numHex5 = String((convert16bits(pkgRead[49], pkgRead[48])), HEX);
    numHex6 = String((convert16bits(pkgRead[115], pkgRead[114])), HEX);
    numHex7 = String((convert16bits(pkgRead[120], pkgRead[119])), HEX);
    numHex8 = String((convert16bits(pkgRead[140], pkgRead[139])), HEX);
    numHex9 = "00";
    numHex10 = "00";
    numHex11 = "00";

    switch ( numHex0.length() ){
        case 1:
            numHex0 = "0" + numHex0;
            break;
        case 2:
            break;
    }
    switch ( numHex1.length() ){
        case 1:
            numHex1 = "0" + numHex1;
            break;
        case 2:
            break;
    }
    switch ( numHex2.length() ){
        case 1:
            numHex2 = "0" + numHex2;
            break;
        case 2:
            break;
    }
    switch ( numHex3.length() ){
        case 1:
            numHex3 = "0" + numHex3;
            break;
        case 2:
            break;
    }
    switch ( numHex4.length() ){
        case 1:
            numHex4 = "0" + numHex4;
            break;
        case 2:
            break;
    }
    switch ( numHex5.length() ){
        case 1:
            numHex5 = "0" + numHex5;
            break;
        case 2:
            break;
    }
    switch ( numHex6.length() ){
        case 1:
            numHex6 = "0" + numHex6;
            break;
        case 2:
            break;
    }
    switch ( numHex7.length() ){
        case 1:
            numHex7 = "0" + numHex7;
            break;
        case 2:
            break;
    }
    switch ( numHex8.length() ){
        case 1:
            numHex8 = "0" + numHex8;
            break;
        case 2:
            break;
    }
    switch ( numHex9.length() ){
        case 1:
            numHex9 = "0" + numHex9;
            break;
        case 2:
            break;
    }
    switch ( numHex10.length() ){
        case 1:
            numHex10 = "0" + numHex10;
            break;
        case 2:
            break;
    }
    switch ( numHex11.length() ){
        case 1:
            numHex11 = "0" + numHex11;
            break;
        case 2:
            break;
    }
    pkgToSend = numHex0 + numHex1 + numHex2 + numHex3 + numHex4 + numHex5 + numHex6 + numHex7 + numHex8 + numHex9 + numHex10 + numHex11;
}

void updateSerial(){
    delay(500);
    while (Serial.available()){
        gsmSerial.write(Serial.read());
    }

    while (gsmSerial.available()){
        Serial.write(gsmSerial.read());
    }
}

void receivedMSG(){
    delay(500);
    String comp = "";

    if ( gsmSerial.available() > 0 ){
        String msgGSM = "";

        while ( gsmSerial.available() > 0 ){
            char letra = gsmSerial.read();
            if ( isAlphaNumeric(letra) ){
                msgGSM += letra;
            }
        }

        if ( msgGSM[29] == 'R' ){
            // Remove o numero de quem esta eviando
            valuesToChange = msgGSM.substring(30);
            enviaGSM = true;
        }
    }
}

void changeParameters(String valuesToChange){
    int address, valueMSB, valueLSB = 0;
    byte pkgWrite[] = {5, 100, 16, 196, 1, 0, 1, 0, 143, 95, 192, 192, 5, 41, 2, 23, 1, address, valueMSB, valueLSB, 0, 191, 61};

    // Pacote de escrita para endereço 03
    // {5, 100, 16, 196, 1, 0, 1, 0, 38, 151, 192 ,192, 5, 41, 2, 23, 1, address, valueMSB, valueLSB, 0 ,47, 104 };

    String lsbRLV, msbRLV, lsbRLI, msbRLI, tempRLV, tempRLI, forTest, strRLV, strRLI = "";

    String vRef = getValue(valuesToChange, 'F', 0);
    String insensitivity = getValue(valuesToChange, 'F', 1);
    String bloqTapMax = getValue(valuesToChange, 'F', 2);
    String bloqTapMin = getValue(valuesToChange, 'F', 3);
    String timer = getValue(valuesToChange, 'F', 4);
    String RLV = getValue(valuesToChange, 'F', 5); //  relationship TP
    String RLI = getValue(valuesToChange, 'F', 6); //  relationship TC

    // Change reference voltage
    pkgWrite[17] = 1;
    pkgWrite[18] = vRef.toInt();
    pkgWrite[19] = 0;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    // Change insensitivity
    pkgWrite[17] = 2;
    pkgWrite[18] = insensitivity.toInt();
    pkgWrite[19] = 0;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    // Change bloqTapMax
    pkgWrite[17] = 13;
    pkgWrite[18] = bloqTapMax.toInt();
    pkgWrite[19] = 0;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    // Change bloqTapMin
    pkgWrite[17] = 14;
    pkgWrite[18] = bloqTapMin.toInt();
    pkgWrite[19] = 0;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    //  timer change
    pkgWrite[17] = 3;
    pkgWrite[18] = timer.toInt();
    pkgWrite[19] = 0;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    // Change RLV
    int intRLV = RLV.toInt();
    int converted_msbRLV, converted_lsbRLV = 0;
    forTest = "";
    strRLV = "";

    strRLV = String(intRLV, HEX);

    if (strRLV.length() == 4){
        msbRLV = strRLV.substring(2);
        converted_msbRLV = int(hexToDec(msbRLV));

        lsbRLV = strRLV.substring(0, 2);
        converted_lsbRLV = int(hexToDec(lsbRLV));
    }

    if (strRLV.length() == 3){
        msbRLV = strRLV.substring(1);
        converted_msbRLV = int(hexToDec(msbRLV));

        lsbRLV = strRLV.substring(0, 1);
        converted_lsbRLV = int(hexToDec(lsbRLV));
    }

    if (strRLV.length() == 2){
        converted_msbRLV = int(hexToDec(strRLV));
        converted_lsbRLV = 0;
    }

    pkgWrite[17] = 15;
    pkgWrite[18] = converted_msbRLV;
    pkgWrite[19] = converted_lsbRLV;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    // Change RLI
    int intRLI = RLI.toInt();
    int converted_msbRLI, converted_lsbRLI = 0;
    forTest = "";
    strRLV = "";

    strRLI = String(intRLI, HEX);

    if (strRLI.length() == 4){
        msbRLI = strRLI.substring(2);
        converted_msbRLI = int(hexToDec(msbRLI));

        lsbRLI = strRLI.substring(0, 2);
        converted_lsbRLI = int(hexToDec(lsbRLI));
    }

    if (strRLI.length() == 3){
        msbRLI = strRLI.substring(1);
        converted_msbRLI = int(hexToDec(msbRLI));

        lsbRLI = strRLI.substring(0, 1);
        converted_lsbRLI = int(hexToDec(lsbRLI));
    }

    if (strRLI.length() == 2){
        converted_msbRLI = int(hexToDec(strRLI));
        converted_lsbRLI = 0;
    }

    pkgWrite[17] = 16;
    pkgWrite[18] = converted_msbRLI;
    pkgWrite[19] = converted_lsbRLI;
    Serial.write(pkgWrite, sizeof(pkgWrite));

    delay(800);

    gsmSerial.println("AT+CMGS=\"+5548998219908\""); //change ZZ with country code and xxxxxxxxxxx with phone number to sms
    delay(300);
    gsmSerial.print("Trocando..."); //text content
    delay(300);
    gsmSerial.write(26);
    delay(300);

    digitalWrite(ledstat, LOW);
}

String getValue(String data, char separator, int index){
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for ( int i = 0; i <= maxIndex && found <= index; i++ ){
        if ( data.charAt(i) == separator || i == maxIndex ){
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

unsigned int hexToDec(String hexString){
    unsigned int decValue = 0;
    int nextInt;
    for ( int i = 0; i < hexString.length(); i++ ){
        nextInt = int(hexString.charAt(i));
        if (nextInt >= 48 && nextInt <= 57)
            nextInt = map(nextInt, 48, 57, 0, 9);
        if (nextInt >= 65 && nextInt <= 70)
            nextInt = map(nextInt, 65, 70, 10, 15);
        if (nextInt >= 97 && nextInt <= 102)
            nextInt = map(nextInt, 97, 102, 10, 15);
        nextInt = constrain(nextInt, 0, 15);
        decValue = (decValue * 16) + nextInt;
    }
    return decValue;
}

void loop(){

    if ( (lersigfox == true) && (enviaGSM == false)  ){
        index = 0;
        Serial.println("Sending...");
        Serial.write(pkgWrite, sizeof(pkgWrite));
        tempoenvio = 0;
        lersigfox = false;
    }
    if ( gsmSerial.available() ){
        receivedMSG();
    }
    if ( (enviaGSM == true) && (lersigfox == false) ){
        digitalWrite(ledstat, HIGH);
        changeParameters(valuesToChange);
        enviaGSM = false;
        valuesToChange = "";
    }
}