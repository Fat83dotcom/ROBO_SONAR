// esboço do software robo fat83.ino

// Bibliotecas para o servo motor e o sensor ultrasonico.
#include <Servo.h>
#include <NewPing.h>
#include <Arduino.h>

// Diretivas para os pinos do Arduino.
#define TRIGGER_PIN   5
#define ECHO_PIN      4 
#define MAX_DISTANCE 300 // Distancia maxima de leitura, acima desta distancio o retorno é zero.
#define motorA1Frente 11
#define motorA2Tras 12
#define motorB1Frente 3
#define motorB2Tras 2
#define SERVO 6 // Porta Digital 6 PWM
#define bipbip 13

// Instanciação dos objetos NewPing e Servo.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
Servo s; // Variável Servo

// Variável global para medir a distancia.
unsigned long dist;
 
void setup() {
  	s.attach(SERVO); // Inicia o pino 6.
  	s.write(90); // Inicia servomotor na posição zero
  	pinMode(motorA1Frente, OUTPUT);
	pinMode(motorA2Tras, OUTPUT);
	pinMode(motorB1Frente, OUTPUT);
	pinMode(motorB2Tras, OUTPUT);
	pinMode(bipbip, OUTPUT);
	for(int i = 0; i < 3; i ++){
		bip(30);	    
	}
	Serial.begin(9600);
}
void Frente(int veloPWM) {
	analogWrite(motorA1Frente, veloPWM);
	digitalWrite(motorA2Tras, 0);
	analogWrite(motorB1Frente, veloPWM);
	digitalWrite(motorB2Tras, 0);
}
void Tras() {
	digitalWrite(motorA1Frente, 0);
	digitalWrite(motorA2Tras, 1);
	digitalWrite(motorB1Frente, 0);
	digitalWrite(motorB2Tras, 1);
}
void Parado() {
	digitalWrite(motorA1Frente, 0);
	digitalWrite(motorA2Tras, 0);
	digitalWrite(motorB1Frente, 0);
	digitalWrite(motorB2Tras, 0);
}
void Direita(int fatorDirecao) {

	// O parametro fatorDirecao define o grau em que o veiculo fara uma conversão.

	digitalWrite(motorA1Frente, 0);
	digitalWrite(motorA2Tras, 1);
	digitalWrite(motorB1Frente, 1);
	digitalWrite(motorB2Tras, 0);
	delay(fatorDirecao);
}
void Esquerda(int fatorDirecao) {

	// O parametro fatorDirecao define o grau em que o veiculo fara uma conversão.

	digitalWrite(motorA1Frente, 1);
	digitalWrite(motorA2Tras, 0);
	digitalWrite(motorB1Frente, 0);
	digitalWrite(motorB2Tras, 1);
	delay(fatorDirecao);
}
void servoMDireita() {
	s.write(0);
}
void servoMEsquerda() {
	s.write(180);
}
void servoMCentro() {
	s.write(90);
}	
void servoMRotacao(int valor) {
	s.write(valor);
}
unsigned long leituraDireita() {

	/* Esta função direciona o servo motor para o lado direito, faz uma leitura da distancia
	e retorna o valor da leitura, que posteriormente será comparada para tomada de decisão.*/

	static unsigned long leituraDistDireita;

	servoMDireita();
	delay(500);
	leituraDistDireita = sonar.ping_cm();
	delay(50);
	bip(30);
	Serial.println("leituraDistDireita");
	Serial.println(leituraDistDireita);
	delay(1000);
	return leituraDistDireita;
}
unsigned long leituraEsquerda() {

	/* Esta função direciona o servo motor para o lado esqerda, faz uma leitura da distancia
	e retorna o valor da leitura, que posterioemtne será comparada para tomada de decisão.*/

	static unsigned long leituraDistEsquerda;

	servoMEsquerda();
	delay(500);
	leituraDistEsquerda = sonar.ping_cm();
	delay(50);
	bip(30);
	Serial.println("leituraDistEsquerda");
	Serial.println(leituraDistEsquerda);
	delay(1000);
	return leituraDistEsquerda;
}
void paraFrente() {

	// Esta função faz o robo ir para frente até que um obstaculo esteja a sua frente.
	servoMCentro();
	Frente(200); // Esta função aceita um sinal PWM entre 0 e 255 para aumentar ou diminuir a rotaçao dos motores.
}
void saida() {
	/*Caso o robô entre em um lugar onde ele não possa virar, ele andará 500 ms para tras
	e chamará está função que fará uma nova leitura, até que ele tenha espaço para seguir em frente.*/
	
	cerebroFat83();
}
void cerebroFat83() {

	if(dist <= 30 && dist != 0) {

		Parado();
		unsigned long d, e;
		unsigned int fatorDirecaoGeral = 220;
		
		for(int i = 0; i < 4; i ++){
		bip(30);	    
		}

		d = leituraDireita();
		e = leituraEsquerda();

		if((d == 0 && e == 0) || d == e) {

			Direita(400);
			servoMCentro();
			Parado();
			for(int i = 0; i < 5; i ++){
				bip(30);
				bip(50);
			}
		}
		else if(d == 0 && e != 0) {
			Direita(fatorDirecaoGeral);
			servoMCentro();
			Parado();
			for(int i = 0; i < 3; i ++){
				bip(30);	    
			}
		}
		else if(e == 0 && d != 0) {
			Esquerda(fatorDirecaoGeral);
			servoMCentro();
			Parado();
			for(int i = 0; i < 2; i ++){
				bip(30);	    
			}
		}
		else if(d <= 10 && e <= 10 && d != 0 && e != 0){
			Tras();
			servoMCentro();
			bip(500);
			Parado();
			delay(50);
			saida();
		}
		else if(d < e) {
			Esquerda(fatorDirecaoGeral);
			servoMCentro();
			Parado();
			for(int i = 0; i < 2; i ++){
				bip(30);	    
			}
		}
		else {
			Direita(fatorDirecaoGeral);
			servoMCentro();
			Parado();
			for(int i = 0; i < 3; i ++){
				bip(30);	    
			}
		}
	}
	else{
		
		paraFrente();
	}
}
void leituraDistancia() {

	dist = sonar.ping_cm();
	Serial.println(dist);
	delay(50);
} 
void bip(int time) {
	digitalWrite(bipbip, 1);
	delay(time);
	digitalWrite(bipbip, 0);
	delay(time);
}
void loop() {

	leituraDistancia();
	cerebroFat83();
}	 