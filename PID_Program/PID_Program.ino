double kp = 2;
double ki = 5;
double kd = 1;

unsigned long currentTime, previousTime;
double elapsedTime;

double errorLeft;
double lastErrorLeft;
double inputLeft, outputLeft;
double cumErrorLeft;
double rateErrorLeft;

double errorRight;
double lastErrorRight;
double inputRight, outputRight;
double cumErrorRight;
double rateErrorRight;

double setPoint = 0;

const int enAPin = 6;
const int in1Pin = 7;
const int in2Pin = 5;
const int in3Pin = 4;
const int in4Pin = 2;
const int enBPin = 3;

void setup() {
    pinMode(enAPin, OUTPUT);
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(in3Pin, OUTPUT);
    pinMode(in4Pin, OUTPUT);
    pinMode(enBPin, OUTPUT);
    
    Serial.begin(9600);
}

void loop() {
    inputLeft = analogRead(A0);
    inputRight = analogRead(A1);
    
    outputLeft = computePID(inputLeft, true);
    outputRight = computePID(inputRight, false);
    
    moveMotors(outputLeft, outputRight);
    
    printDebugInfo();
    
    delay(100);
}

double computePID(double inp, bool isLeft) {     
    currentTime = millis();
    elapsedTime = (double)(currentTime - previousTime);
    
    if (isLeft) {
        errorLeft = setPoint - inp;
        cumErrorLeft += errorLeft * elapsedTime;
        rateErrorLeft = (errorLeft - lastErrorLeft)/elapsedTime;
        
        double out = kp*errorLeft + ki*cumErrorLeft + kd*rateErrorLeft;
        
        lastErrorLeft = errorLeft;
        previousTime = currentTime;
        
        return constrain(out, -255, 255);
    } else {
        errorRight = setPoint - inp;
        cumErrorRight += errorRight * elapsedTime;
        rateErrorRight = (errorRight - lastErrorRight)/elapsedTime;
        
        double out = kp*errorRight + ki*cumErrorRight + kd*rateErrorRight;
        
        lastErrorRight = errorRight;
        previousTime = currentTime;
        
        return constrain(out, -255, 255);
    }
}

void moveMotors(double leftOutput, double rightOutput) {
    if (leftOutput >= 0) {
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
    } else {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
        leftOutput = -leftOutput;
    }
    
    if (rightOutput >= 0) {
        digitalWrite(in3Pin, HIGH);
        digitalWrite(in4Pin, LOW);
    } else {
        digitalWrite(in3Pin, LOW);
        digitalWrite(in4Pin, HIGH);
        rightOutput = -rightOutput;
    }
    
    analogWrite(enAPin, leftOutput);
    analogWrite(enBPin, rightOutput);
}

void printDebugInfo() {
    Serial.println("=== Informasi Debug Kontrol PID ===");
    
    Serial.println("Left Motor:");
    Serial.print("Input: ");
    Serial.println(inputLeft);
    Serial.print("Error: ");
    Serial.println(errorLeft);
    Serial.print("Output: ");
    Serial.println(outputLeft);
    
    Serial.println("\nRight Motor:");
    Serial.print("Input: ");
    Serial.println(inputRight);
    Serial.print("Error: ");
    Serial.println(errorRight);
    Serial.print("Output: ");
    Serial.println(outputRight);
    
    Serial.println("========================\n");
}
