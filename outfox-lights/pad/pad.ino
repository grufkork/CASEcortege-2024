void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  Serial.begin(115200);
}

int pins[] = {A1, A2, A3, A4};

bool key[] = {false, false, false, false};

int cooldown[] = {0, 0, 0, 0};

float avgs[] = {512.0f, 512.0f, 512.0f, 512.0f};

int val;

void loop() {
  //val = analogRead(A2);
  //avgs[0] = (avgs[0]+val)/2.0f;
  //Serial.println(avgs[0]);
  //return;

  for(int i = 0; i < 4; i++){
    val = analogRead(pins[i]);

    avgs[i] = (avgs[i]+val*10.0f)/11.0f;

    /*if(cooldown[i] > 0){
      cooldown[i]--;
      continue;
    }*/

    if(avgs[i] >= 1000 && !key[i]){
      if(cooldown[i] > 5){
        key[i] = true;
        Serial.write(0b10000000 + i);
        Serial.flush();
      }else{
        cooldown[i] ++;
      }
    }else if(avgs[i] <= 20 && key[i]){
      if(cooldown[i] > 5){
        key[i] = false;
        Serial.write(0b00000000 + i);
        Serial.flush();
      }else{
        cooldown[i] ++;
      }
    }else{
      cooldown[i]=0;
    }
  }

  
}
