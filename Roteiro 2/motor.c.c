
if(pta4==0){ // botao S1 apertado -> duty_cycle zera.
  duty_cycle = 0;
  TPM1_C0V =  13107 *( 1 - (duty_cycle/100) );
}
else if(pta5==0){ // botao S2 apertado -> diminuicao do duty_cycle.
  if(duty_cycle>0){
    duty_cycle = duty_cycle - 5;
    TPM1_C0V =  TPM1_C0V + 655;
  }
}
else if(pta12==0){ // botao S3 apertado -> aumento do duty_cycle.
  if(duty_cycle<100){
    duty_cycle = duty_cycle + 5;
    TPM1_C0V =  TPM1_C0V - 655;
  }
}

void dir(*string){
  pte29 = ; // Leitura da GPIO
  pte30 = ; // Leitura da GPIO
  if(string[0] == 'V' && string[1] == 'e' && string[2] == 'n' && string[3] == 't'){
    pte29 = 1;
    pte30 = 0;
  }
  if(string[0] == 'E' && string[1] == 'x' && string[2] == 'a' && string[3] == 'u' && string[0] == 's' && string[1] == 't'){
    pte29 = 0;
    pte30 = 1;
  }
}

void power(*string){
  int u=0, d=0, c=0;
  if(string[2] != '\0'){
    u = string[2];
    d = string[1]*10;
    c = string[0]*100;
  }
  else if(string[1] != '\0'){
    u = string[1];
    d = string[0]*10;
  }
  else{
    u = string[0];
  }
  duty_cycle = u + d + c;
}

void mode(*string){
  pte29 = ; // Leitura da GPIO
  pte30 = ; // Leitura da GPIO
  if(string[0] == 'O' && string[1] == 'n'){ // On
    pte29 = 1;
    pte30 = 1;
  }
  if(string[0] == 'O' && string[1] == 'f' && string[2] == 'f'){ // Off
    pte29 = 0;
    pte30 = 0;
  }
  if(string[0] == 'A' && string[1] == 'u' && string[2] == 't' && string[3] == 'o'){ // Auto
    thr = threshold();
    if(temp > thr){ // Liga
      pte29 = 1;
      pte30 = 1;
    }
    else{ // Desliga
      pte29 = 0;
      pte30 = 0;
    }
  }
}

int threshold(*string){
  int n = 0;
  int u, d, c;
  while(string[n] != '\0'){
    if(string[n] != '.'){
      c = string[n];
      d = string[n+1];
      u = string[n+2];
    }
    n++;
  }
  return value;
}
