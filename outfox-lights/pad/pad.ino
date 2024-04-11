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
float maxs[] = {1.0f, 1.0f, 1.0f, 1.0f};

int val;

void loop() {
  //val = analogRead(A1);
  //avgs[0] = (avgs[0]+val)/2.0f;
  //Serial.println(val);
  //return;

  for(int i = 0; i < 4; i++){
    val = analogRead(pins[i]);

    avgs[i] = (avgs[i]+val-509)*0.9f;

    /*if(cooldown[i] > 0){
      cooldown[i]--;
      continue;
    }*/

    if(key[i]){
      if(maxs[i] < avgs[i]){
        maxs[i] = avgs[i];
      }
    }

    if(avgs[i] >= 450.0f && !key[i]){
      maxs[i] = 450.0f;
      key[i] = true;
      Serial.write(0b10000000 + i);
      Serial.flush();
      /*
      if(cooldown[i] > 5){
      }else{
        cooldown[i] ++;
      }*/
    }else if(key[i] && avgs[i] <= -170.0f * maxs[i] / 450.0f ){
      key[i] = false;
      Serial.write(0b00000000 + i);
      Serial.flush();
      /*if(cooldown[i] > 5){
      }else{
        cooldown[i] ++;
      }*/
    }
    
  }

  
}
