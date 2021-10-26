/* Questo sketch è caricato nell'arduino uno e si occupa semplicemente di recepire degli ingressi sui pin A0,A1,A2,A3, che possono essere impostati
per ricevere segnali digitali, e visualizzare su uno schermo lcd il risultato del riconoscimento della parola, nessuna logica di machine learning è inclusa in questo sketch */

//importo la libreria necessaria all'utilizzo del display lcd 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); //dichiaro l'indirizzo I2C del display, recuperato con uno sketch dedicato, e il numero di righe e colonne del display utilizzato

void setup() {
  lcd.init(); // inizializzo il display
  lcd.backlight(); // imposto ad ON la retroilluminazione del display
  pinMode(A0, INPUT); // pin dove ricevo il segnale del riconoscimento della parola "unknow"
  pinMode(A1, INPUT); // pin dove ricevo il segnale del riconoscimento della parola "daniele"
  pinMode(A2, INPUT); // pin dove ricevo il segnale del riconoscimento della parola "no"
  pinMode(A3, INPUT); // pin dove ricevo il segnale del riconoscimento della parola "si"
}


// in questa funzione sono presenti i 4 casi, in funzione del segnale ricevuto dal pin visualizzerò sullo schermo il risultato
void receiveEvent() {
   
      if(digitalRead(A3)==HIGH){
      lcd.setCursor(0,0);
      lcd.print("Riconosciuto:");
      lcd.setCursor(0,1);
      lcd.print("SI");
      delay(2000);
      lcd.clear();
   }
   
      if(digitalRead(A2)==HIGH){
      lcd.setCursor(0,0);
      lcd.print("Riconosciuto:");
      lcd.setCursor(0,1);
      lcd.print("NO");
      delay(2000);
      lcd.clear();
   }
   
      if(digitalRead(A1)==HIGH){
      lcd.setCursor(0,0);
      lcd.print("Riconosciuto:");
      lcd.setCursor(0,1);
      lcd.print("DANIELE");
      delay(2000);
      lcd.clear();
   }
   
      if(digitalRead(A0)==HIGH){
      lcd.setCursor(0,0);
      lcd.print("Riconosciuto:");
      lcd.setCursor(0,1);
      lcd.print("Sconosciuto");
      delay(2000);
      lcd.clear();
   }
}

void loop() {
  receiveEvent();
  delay(100);
}
