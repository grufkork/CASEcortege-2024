void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(115200);
}

int pins[] = {A1, A2, A3, A4};

bool key[] = {false, false, false, false};

int cooldown[] = {0, 0, 0, 0};

float avgs[] = {0.0f, 0.0f, 0.0f, 0.0f};
float maxs[] = {1.0f, 1.0f, 1.0f, 1.0f};

int val;

bool enterKey = false;
bool escKey = false;

void loop() {
  //val = analogRead(A1);
  //avgs[0] = (avgs[0]+val)/2.0f;
  //Serial.println(val);
  //return;

  if(enterKey != digitalRead(2)){
    if(enterKey){
      Serial.write(0b10000000 + 4);
    }else{
      Serial.write(0b00000000 + 4);
    }
    Serial.flush();
    enterKey = !enterKey;
  }

  if(escKey != digitalRead(3)){
    if(escKey){
      Serial.write(0b10000000 + 5);
    }else{
      Serial.write(0b00000000 + 5);
    }
    Serial.flush();
    escKey = !escKey;
  }

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
    }else if(key[i] && avgs[i] <= -170.0f * maxs[i] / 650.0f ){
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
