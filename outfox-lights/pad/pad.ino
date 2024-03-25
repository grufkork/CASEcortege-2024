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

float avgs[] = {0.0f, 0.0f, 0.0f, 0.0f};

int val;

void loop() {
  //val = analogRead(A1);
  //avgs[0] = (avgs[0]+val)/2.0f;
  //Serial.println(val);
  //return;

  for(int i = 0; i < 4; i++){
    val = analogRead(pins[i]);

    avgs[i] = (avgs[i]+val-509)*0.99f;

    /*if(cooldown[i] > 0){
      cooldown[i]--;
      continue;
    }*/

    if(avgs[i] >= 3000.0f && !key[i]){
      key[i] = true;
      Serial.write(0b10000000 + i);
      Serial.flush();
      /*
      if(cooldown[i] > 5){
      }else{
        cooldown[i] ++;
      }*/
    }else if(avgs[i] <= -4000.0f && key[i]){
      key[i] = false;
      Serial.write(0b00000000 + i);
      Serial.flush();
      /*if(cooldown[i] > 5){
      }else{
        cooldown[i] ++;
      }*/
    }else{
      cooldown[i]=0;
    }
  }

  
}
