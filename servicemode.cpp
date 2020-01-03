// byte reset_idx  __attribute__ ((section (".noinit"))); // do not clear this RAM at startup
// void setup() {
//   Serial.begin(57600);
//  if (MCUSR & 0x02){ Serial.println("reset source bit1: External Reset Flag");reset_idx++;} // increment at each action on RESET button
//  if (MCUSR & 0x04){ Serial.println("reset source bit2: Brown-out Reset Flag");reset_idx+=10;}
//  if (MCUSR & 0x08){ Serial.println("reset source bit3: Watchdog System Reset Flag\n");reset_idx+=20;}
//  if (MCUSR & 0x01){ Serial.println("reset source bit0: Power-on Reset Flag"); reset_idx=1;} // after power-ON, reset this option to value 1
//  MCUSR=0;
//  Serial.print("Reset index ");Serial.println(reset_idx); 
//  pinMode(13,OUTPUT);// onboard led
// }

// void loop() {
//   // blink: 1 flash after power-UP, and 1 more flash after each action on reset button
//   for(byte i=0;i<reset_idx;i++){
//     digitalWrite(13,1);
//     delay (80);
//     digitalWrite(13,0);
//     delay (80);  
//   }
//   delay (1000); 
// }