//#define DEBUG

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

int n = 0;

const float PRESSFORCE = 1500.0f;
const float RELEASEFORCE = 1000.0f;
const float PRESSCOMPENSATION = 1.2f;
const float RELEASEFORCE_COMPENSATED = RELEASEFORCE / (PRESSFORCE * PRESSCOMPENSATION);


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

    avgs[i] = (avgs[i]+val-509)*0.95f;

    #ifdef DEBUG
    if(n <= 0 && i == 0){
      Serial.println(avgs[i]);
      n=50;
    }
    n--;
    #endif
    /*if(cooldown[i] > 0){
      cooldown[i]--;
      continue;
    }*/

    // While a key is held, store the highest value.
    // This is later used to scale the release sensitivity, so a harder press requires a harder release 
    if(key[i]){
      if(maxs[i] < avgs[i]){
        maxs[i] = avgs[i];
      }
    }

    if(avgs[i] >= 1500.0f && !key[i]){
      maxs[i] = 1500.0f; // Reset the max level to a baseline
      key[i] = true;
      #ifndef DEBUG
      Serial.write(0b10000000 + i);
      Serial.flush();
      #endif
      /*
      if(cooldown[i] > 5){
      }else{
        cooldown[i] ++;
      }*/
    }else if(key[i] && avgs[i] <= -RELEASEFORCE_COMPENSATED * maxs[i] ){
      key[i] = false;
      #ifndef DEBUG
      Serial.write(0b00000000 + i);
      Serial.flush();
      #endif
      /*if(cooldown[i] > 5){
      }else{
        cooldown[i] ++;
      }*/
    }
    
  }

  
}
