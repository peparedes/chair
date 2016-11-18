// A0, A1 are pre-defined as analaog inputs for first row and col.
// Note A5,A6 are INACTIVE.
#define BUTT1_PIN A2
#define BUTT2_PIN A3
#define BUTT3_PIN A4
#define BUTT4_PIN A5
#define BUTT5_PIN A6

// 2-dimensional array of row pin numbers:
const byte colPins[7] = {2, 3, 4, 5, 6, 7, A1};
// 2-dimensional array of column pin numbers:
const byte rowPins[7] = {8, 9, 10, 11, 12, 13, A0};

/*
 * Pre-defined dimensions of the matrix of motors.
 */
const int mat[7][7] = {{1, 1, 1, 1, 1, 1, 1}, {0, 1, 1, 1, 1, 1, 0}, 
{0, 1, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 1, 1, 0}, {1, 1, 1, 1, 1, 1, 1}, 
{1, 0, 1, 1, 1, 0, 1}, {0, 0, 1, 1, 1, 0, 0}};

void setup() {
  Serial.begin(9600);

  // Setup input pins
  pinMode(BUTT1_PIN, INPUT_PULLUP);
  pinMode(BUTT2_PIN, INPUT_PULLUP);
  pinMode(BUTT3_PIN, INPUT_PULLUP);
  pinMode(BUTT4_PIN, INPUT_PULLUP);

  // Setup output pins
  resetPins();
}

/*
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * THIS IS WHERE YOU WILL ASSIGN ACTIONS TO BUTTONS.
 * THERE ARE 8 VIBRATIONS, BUT ONLY 4 BUTTONS.
 * RUN ONLY THE FIRST 4 IF STATEMENTS FIRST.
 * (COMMENT OUT THE SECOND 4).
 * THEN COMMENT OUT THE FIRST 4 AND RUN THE SECOND 4.
 * BUTTON 1 CORRESPONDS TO THE BUTTON ON THE VERY SIDE OF THE REMOTE.
 */
void loop() {
  int time = 200;

  if (digitalRead(BUTT1_PIN) == LOW) {
    sideStretch();
  }

  if (digitalRead(BUTT2_PIN) == LOW) {
    backTwist();
  }

  if (digitalRead(BUTT3_PIN) == LOW) {
    dragonBreath(6, 1); 
  }

  if (digitalRead(BUTT4_PIN) == LOW) { 
    fourCountBreath(5, 2);
  }
}

/*
 * These are the methods reperesenting vibration patterns.
 * Method names are self-explanatory.
 */
 
void sideStretch() {
  fastSwipeOneCol(2,100,50);
  delay(600);
  fastSwipeOneCol(4,100,50);
}

void backTwist() {
  region(1, 2, 1, 2); //LEFT region
  delay(500); //Pause
  region(1, 2, 4, 5); //RIGHT region
}

void bendForwardBackward() {
  fastSwipeTwoColUp(2, 4, 100, 50); //Swipe UP
  delay(500); //Pause
  fastSwipeTwoColDown(2, 4, 100, 50); //Swipe DOWN
}

void liftDropShoulders() {
  fastSwipeTwoColUp(0,2,2,4,100,50); //Swipe UP on shoulders
  delay(400); //Pause
  fastSwipeTwoColDown(0,2,2,4,100,50); //Swipe DOWN on shoulders
  delay(500); //Pause
  fastSwipeTwoColUp(0,2,2,4,100,50); //Repeat
  delay(400);
  fastSwipeTwoColDown(0,2,2,4,100,50);
}

void turnHead() {
  fastSwipeLeftToRight(0,1,1,5,100,50);
  delay(500);
  fastSwipeRightToLeft(0,1,1,5,100,50);
}

void turnHips() {
  fastSwipeLeftToRight(5,6,1,5,100,50);
  delay(500);
  fastSwipeRightToLeft(5,6,1,5,100,50);
}

void dragonBreath(byte bottom, byte top) {
  if (bottom >= sizeof(rowPins)) {
    bottom = sizeof(rowPins) - 1;
  }  
  if (top < 0) {
    top = 0;
  }
  
  for (int i = bottom; i > bottom/2; i--) { //in
    resetPins();
    pckge_rowOn(i);
    delay(200);
  }

  resetPins();
  delay(400); 

  for (int i = bottom/2; i >= top; i--) { //in
    resetPins();
    pckge_rowOn(i);
    delay(200);
  }

  resetPins();
  delay(800);
  swipeAllRowsTopToBottom(90, 40);
  resetPins(); // Turn all off when done
  delay(300);
}

void fourCountBreath(byte bottom, byte top) {
  if (bottom >= sizeof(rowPins)) {
    bottom = sizeof(rowPins) - 1;
  }  
  if (top < 0) {
    top = 0;
  }
  
  for (int i = bottom; i >= top; i--) { //inhale
    resetPins();
    pckge_rowOn(i);
    delay(350);
  }

  for (int j = 0; j < 3; j++) { //hold
    resetPins();
    delay(700);
    pckge_rowOn(top);
    delay(300);
  }
  resetPins();
  delay(700); 
  
  for (int i = top; i <= bottom; i++) { //exhale
    resetPins();
    pckge_rowOn(i);
    delay(350);
  }

  for (int j = 0; j < 2; j++) { //hold
    resetPins();
    delay(700);
    pckge_rowOn(bottom);
    delay(300);
  }
  resetPins(); // Turn all off when done
  delay(700); 
}

/*
 * Below are methods for vibration patterns.
 * These are the "building blocks" of the vibrations we test.
 * Scroll to top to see description of each.
 */
 
void fastSwipeOneCol(int col, int dur_t, int delay_t) {
  
  /* Turn on all columns */
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
  }
  
  /* Enter the Overlap Mission */
for (int j = 0; j < sizeof(rowPins)-1; j++) {
    //pinMode(rowPins[j], OUTPUT);
    digitalWrite(rowPins[j], HIGH);
    resetColsExceptFor(col);
    delay(dur_t);
    //pinMode(rowPins[j+1], OUTPUT);
    digitalWrite(rowPins[j+1], HIGH);
    resetColsExceptFor(col);
    delay(delay_t);
    //pinMode(rowPins[j], OUTPUT);    
    digitalWrite(rowPins[j], LOW);
    resetColsExceptFor(col);
    resetRowsExceptFor(j+1);
  }

  /* Optional: reset all pins after each mission*/
  resetPins();
}

void fastSwipeTwoColDown(byte col1, byte col2, int dur_t, int delay_t) {
  resetColsExceptFor(col1,col2);

  for (int j = 0; j < sizeof(rowPins); j++) {
    if(j == 0) { // first index
      //Phase 1
      rowOn(j);
      delay(dur_t);

      //Phase 2
      rowOn(j+1);
      delay(delay_t);

      //Phase 3
      rowOff(j);
    }
    
    if((j > 0) && (j < sizeof(rowPins)-1)) { // in betweens
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j-1);
      rowOn(j+1);
      delay(delay_t);     
    }
    
    if(j == sizeof(rowPins)-1) { // last index 
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j-1);
      delay(delay_t);
    }
  }
  resetPins();
}

void fastSwipeTwoColUp(byte col1, byte col2, int dur_t, int delay_t) {
  resetColsExceptFor(col1,col2);

  for (int j = sizeof(rowPins) - 1; j >=0; j--) {
    if(j == sizeof(rowPins) - 1) { // first index
      //Phase 1
      rowOn(j);
      delay(dur_t);

      //Phase 2
      rowOn(j-1);
      delay(delay_t);

      //Phase 3
      rowOff(j);
    }
    
    if((j > 0) && (j < sizeof(rowPins)-1)) { // in betweens
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j+1);
      rowOn(j-1);
      delay(delay_t);     
    }
    
    if(j == 0) { // last index 
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j+1);
      delay(delay_t);
    }
  }
  resetPins();
}

void fastSwipeTwoColDown(byte row1, byte row2, byte col1, byte col2, int dur_t, int delay_t) {
  resetColsExceptFor(col1,col2);

  for (int j = row1; j <= row2; j++) {
    if(j == row1) { // first index
      //Phase 1
      rowOn(j);
      delay(dur_t);

      //Phase 2
      rowOn(j+1);
      delay(delay_t);

      //Phase 3
      rowOff(j);
    }
    
    if((j > row1) && (j < row2)) { // in betweens
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j-1);
      rowOn(j+1);
      delay(delay_t);     
    }
    
    if(j == row2) { // last index 
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j-1);
      delay(delay_t);
    }
  }
  resetPins();
}

void fastSwipeTwoColUp(byte row1, byte row2, byte col1, byte col2, int dur_t, int delay_t) {
  resetColsExceptFor(col1,col2);

  for (int j = row2; j >= row1; j--) {
    if(j == row2) { // first index
      //Phase 1
      rowOn(j);
      delay(dur_t);

      //Phase 2
      rowOn(j-1);
      delay(delay_t);

      //Phase 3
      rowOff(j);
    }
    
    if((j > row1) && (j < row2)) { // in betweens
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j+1);
      rowOn(j-1);
      delay(delay_t);     
    }
    
    if(j == row1) { // last index 
      //Phase 1
      rowOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      rowOff(j+1);
      delay(delay_t);
    }
  }
  resetPins();
}

void swipeAllRowsTopToBottom(int dur_t, int delay_t) {

  /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    colOn(i);
  }
  for (int j = 0; j < sizeof(rowPins)-1; j++) {
    if(j == 0) {
      rowOn(j);
      delay(dur_t);
      rowOn(j+1);
      delay(delay_t);
      rowOff(j);
    }
    else {
      rowOn(j);
      delay(dur_t - delay_t);
      rowOn(j+1);
      delay(delay_t);
      rowOff(j);
    }
  }
  /* Optional: reset all pins after each mission */
  resetPins(); 
}

void swipeAllRowsBottomToTop(int dur_t, int delay_t) {

  /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    colOn(i);
  }
  for (int j = sizeof(rowPins)-1; j >= 0; j--) {
    if(j == sizeof(rowPins)-1) {
      rowOn(j);
      delay(dur_t);
      rowOn(j-1);
      delay(delay_t);
      rowOff(j);
    }
    else {
      rowOn(j);
      delay(dur_t - delay_t);
      rowOn(j-1);
      delay(delay_t);
      rowOff(j);
    }
  }
  /* Optional: reset all pins after each mission */
  resetPins(); 
}

void interestIng(int dur_t, int delay_t) {
 /* Turn on all columns */
  for (int i = 0; i < sizeof(colPins); i++) {
    colOn(i);
  }
  for (int j = 1; j < sizeof(rowPins)-1; j++) {
    if(j == 1) {
      rowOn(j);
      delay(dur_t);
      rowOn(j-1);
      delay(delay_t);
      rowOff(j);
    }
    else {
      rowOn(j);
      delay(dur_t - delay_t);
      rowOn(j-1);
      delay(delay_t);
      rowOff(j);
    }
  }
  /* Optional: reset all pins after each mission */
  //resetPins();  
}

void rowTest(byte r) {
  resetPins();
  pckge_rowOn(r);
}

void colTest(byte c) {
  resetPins();
  pckge_colOn(c);
}

void rowOn(byte row) {
  digitalWrite(rowPins[row], HIGH);
}

void colOn(byte col) {
  digitalWrite(colPins[col], LOW);
}

void rowOff(byte row) {
  digitalWrite(rowPins[row], LOW);
}

void colOff(byte col) {
  digitalWrite(colPins[col], HIGH);
}

void multiRowOn(byte startRow, byte endRow){

  // Turn on all the columnscol
  for (int i = 0; i < sizeof(colPins); i++) {
    digitalWrite(colPins[i], LOW);
  }
  
  for(int j = startRow; j < endRow+1; j++){
    digitalWrite(rowPins[j], HIGH);
  }
}

void multiColOn(byte startCol, byte endCol) {
 // Turn on all the rows
  for (int i = 0; i < sizeof(rowPins); i++) {
    digitalWrite(rowPins[i], HIGH);
  }
  
  for(int j = startCol; j < endCol+1; j++){
    digitalWrite(colPins[colPins[j]], LOW);
  }
}

// Turn an entire row on
void pckge_rowOn(byte row) {
  // Turn on all the columns
  for (int i = 0; i < sizeof(colPins); i++) {
    digitalWrite(colPins[i], LOW);
  }
  digitalWrite(rowPins[row], HIGH);
}

//Turn an entire column on
void pckge_colOn(byte col) {
  // Turn on all the columns
  for (int i = 0; i < sizeof(rowPins); i++) {
    digitalWrite(rowPins[i], HIGH);
  }

  digitalWrite(colPins[col], LOW);
}

// version 1 : Row -> Col
void singleton(byte row, byte col) {
  digitalWrite(colPins[col], LOW);
  delay(150);
  resetRowsExceptFor(row);
  delay(150);
}

// version 2 : Col -> Row

void resetRowsExceptFor(int row) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    if (i == row) {
      digitalWrite(rowPins[i], HIGH);
    } else {
      digitalWrite(rowPins[i], LOW);
    }
  }
}

void resetRowsExceptFor(byte row1, byte row2) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    if ((i == row1) || (i == row2)) {
      digitalWrite(rowPins[i], HIGH);
    } else {
      digitalWrite(rowPins[i], LOW);
    }
  }
}

void resetRowsExceptForRegion(byte row1, byte row2) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    if ((i >= row1) || (i <= row2)) {
      digitalWrite(rowPins[i], HIGH);
    } else {
      digitalWrite(rowPins[i], LOW);
    }
  }
}

void resetColsExceptFor(int col) {
digitalWrite(colPins[col], LOW);
  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    if (i == col) {
      digitalWrite(colPins[i], LOW);
    } else {
      digitalWrite(colPins[i], HIGH);
    }  
  }
}

void resetColsExceptFor(byte col1, byte col2) {
  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    if ((i == col1) || (i == col2)) {
      digitalWrite(colPins[i], LOW);
    } else {
      digitalWrite(colPins[i], HIGH);
    }
  }
}

void resetColsExceptForRegion(byte col1, byte col2) {
  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    if ((i >= col1) || (i <= col2)) {
      digitalWrite(colPins[i], LOW);
    } else {
      digitalWrite(colPins[i], HIGH);
    }
  }
}

void region(byte row1, byte row2, byte col1, byte col2) {
  resetColsExceptFor(col1, col2);
  for (int i = 0; i < 2; i++) {
    digitalWrite(rowPins[row2], HIGH);
    digitalWrite(rowPins[row1], HIGH);
    delay(100);
    digitalWrite(rowPins[row2], LOW);
    digitalWrite(rowPins[row1], LOW);
    delay(100);
  } 
  digitalWrite(rowPins[row2], HIGH);
  digitalWrite(rowPins[row1], HIGH);
  delay(400);
   
  resetPins();
  delay(200);
}

// Turns off ALL LEDs
void resetPins() {
  for (int i = 0; i < sizeof(rowPins); i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);
  }

  for (int i = 0; i < sizeof(colPins); i++) {
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

void fastSwipeLeftToRight(byte row1, byte row2, byte col1, byte col2, int dur_t, int delay_t) {
 
  resetRowsExceptFor(row1,row2);
  
  for (int j = col1; j <= col2; j++) {
    if(j == col1) { // first index
      //Phase 1
      colOn(j);
      delay(dur_t);

      //Phase 2
      colOn(j+1);
      delay(delay_t);

      //Phase 3
      colOff(j);
    }
    
    if(j > col1 && j < col2) { // in betweens
      //Phase 1
      colOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      colOff(j-1);
      colOn(j+1);
      delay(delay_t);     
    }
    
    if(j == col2) { // last index 
      //Phase 1
      colOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      colOff(j-1);
      delay(delay_t);
    }
  }
  resetPins();
}

void fastSwipeRightToLeft(byte row1, byte row2, byte col1, byte col2, int dur_t, int delay_t) {
  
  resetRowsExceptFor(row1,row2);

  for (int j = col2; j >= col1; j--) {
    if(j == col2) { // first index
      //Phase 1
      colOn(j);
      delay(dur_t);

      //Phase 2
      colOn(j-1);
      delay(delay_t);

      //Phase 3
      colOff(j);
    }
    
    if(j > col1 && j < col2) { // in betweens
      //Phase 1
      colOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      colOff(j+1);
      colOn(j-1);
      delay(delay_t);     
    }
    
    if(j == col1) { // last index 
      //Phase 1
      colOn(j);
      delay(dur_t-delay_t);

      //Phase 2
      colOff(j+1);
      delay(delay_t);
    }
  }
  resetPins();
}

// wait_ms is the time between switching from one row/colum to another. 
// use a lower value to get the animation to go faster.
void swipeLeftToRight(int wait_ms) {
  for (int i = 0; i < sizeof(colPins); i++) {
    resetPins();
    pckge_colOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

// wait_ms is the time between switching from one row/colum to another.
//use a lower value to get the animation to go faster.
void swipeRightToLeft(int wait_ms) {
  for (int i = sizeof(colPins); i >= 0 ; i--) {
    resetPins();
    pckge_colOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

void swipeTopToBottom(int wait_ms) {
  for (int i = 0; i < sizeof(rowPins); i++) {
    resetPins();
    pckge_rowOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

void swipeBottomToTop(int wait_ms) {
  for (int i = sizeof(rowPins); i >= 0; i--) {
    resetPins();
    pckge_rowOn(i);
    delay(wait_ms);
  }

  resetPins(); // Turn all off when done
}

/* Experimental Sesh */
/* (newly added features) */

void testCol(int time) {
  swipeLeftToRight(time);
}

void testRow(int time) {
  swipeTopToBottom(time);
}

void action_cross(int wait_ms) {
  colOn(3);
  delay(wait_ms);
  colOn(4);
  delay(wait_ms);
  rowOn(3);
  delay(wait_ms);
  rowOn(4);
  delay(wait_ms);
}

void everySingleOne(int wait_ms) {
  //resetPins();
  for (int i = 0; i < sizeof(colPins); i++) {
    for (int j = 0; j < sizeof(rowPins); j++) {
      if(getValueAt(i,j) == 1) {
        singleton(i,j);
        delay(10);
        resetPins();
      }
    }
  }
  resetPins();
}

int getValueAt(int row, int col) {
  return mat[row][col];
}

void swipeUShaped(int wait_ms){
  multiColOn(4,6);
  delay(wait_ms);
  resetPins();
  multiRowOn(0,3);
  delay(wait_ms);
  resetPins();
  multiColOn(0,3);
  delay(wait_ms);
  resetPins();
}
