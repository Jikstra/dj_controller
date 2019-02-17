const byte rows[] = {4, 5};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

const byte cols[] = {8, 9, 10, 11, 12, 13};
const int colCount = sizeof(cols)/sizeof(cols[0]);




void setup() {
  // put your setup code here, to run once:
  for(int i=0; i<rowCount; i++) {
    pinMode(rows[i], INPUT);
    
  }

  for(int i=0; i<colCount; i++) {
    pinMode(cols[i], INPUT_PULLUP);
    
  }
  Serial.begin(115200);
}

void loop() {
  for (int colIndex=0; colIndex < colCount; colIndex++) {
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);

    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
      byte curRow = rows[rowIndex];
      pinMode(curRow, INPUT_PULLUP);
      int val = !digitalRead(curRow);
      pinMode(curRow, INPUT);

      if(val == 1) {
        Serial.println(String(colIndex) + '|' + String(rowIndex) + ' ' +  String(val) );
      }
      
    }

    pinMode(curCol, INPUT);
  }


  
}
