#include <Adafruit_Microbit_Matrix.h>

#define MATRIX_ROWS 3
#define MATRIX_COLS 9
uint8_t rowpins[MATRIX_ROWS] = {26, 27, 28};
uint8_t colpins[MATRIX_COLS] = {3, 4, 10, 23, 24, 25, 9, 7, 6};

uint8_t pixel_to_row[25] = {1, 2, 1, 2, 1, 
			    3, 3, 3, 3, 3, 
			    2, 1, 2, 3, 2,
			    1, 1, 1, 1, 1,
			    3, 2, 3, 2, 3};

uint8_t pixel_to_col[25] = {1, 4, 2, 5, 3,
			    4, 5, 6, 7, 8,
			    2, 9, 3, 9, 1,
			    8, 7, 6, 5, 4,
			    3, 7, 1, 6, 2};

volatile uint8_t currentRow = 0;

Adafruit_Microbit_Matrix *handle = NULL;

/** TIMTER2 peripheral interrupt handler. This interrupt handler is called whenever there it a TIMER2 interrupt
 * Don't mess with this line. really.
 */
 extern "C"  { void TIMER2_IRQHandler(void) { IRQ_MATRIX_HANDLER();  } }



Adafruit_Microbit_Matrix::Adafruit_Microbit_Matrix() :  Adafruit_GFX(5,5) {
  memset(matrix_buffer, 0x0, MATRIX_COLS * MATRIX_ROWS);
}

Adafruit_Microbit_Matrix::~Adafruit_Microbit_Matrix(void) {}

boolean Adafruit_Microbit_Matrix::begin(void) {
  handle = this;

  for (uint8_t c=0; c<MATRIX_COLS ; c++) {
    pinMode(colpins[c], OUTPUT);
    digitalWrite(colpins[c], HIGH);
  }


  for (uint8_t r=0; r<MATRIX_ROWS ; r++) {
    pinMode(rowpins[r], OUTPUT);
    digitalWrite(rowpins[r], LOW);
  }

  for (uint8_t r=0; r<MATRIX_ROWS; r++) {
    for (uint8_t c=0; c<MATRIX_COLS; c++) {
      matrix_buffer[r][c] = 0;
    }
  }

  currentRow = 0;

  startTimer();
  return true;
}

// Matrix object function called by IRQ handler for each row
// This is not optimized at all but its not so bad either!
void Adafruit_Microbit_Matrix::rowHandler(void) {
  // disable current row
  digitalWrite(rowpins[currentRow], LOW);
  for (uint8_t c=0; c<MATRIX_COLS; c++) {
    digitalWrite(colpins[c], HIGH);
  }

  // go to next row
  if(++currentRow >= MATRIX_ROWS) {
    currentRow = 0;
  }

  //Serial.print("Row: "); Serial.println(currentRow);

  for (uint8_t c=0; c<MATRIX_COLS; c++) {
    if (matrix_buffer[currentRow][c]) {
      digitalWrite(colpins[c], LOW);    // Change column states for new row
    }
  }
  digitalWrite(rowpins[currentRow], HIGH);
}


// This sets up the IRQ for timer 2 to run the matrix refresh.
void Adafruit_Microbit_Matrix::startTimer(void)
{    
  NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;              // Set the timer in Counter Mode
  NRF_TIMER2->TASKS_CLEAR = 1;                           // clear the task first to be usable for later
  NRF_TIMER2->PRESCALER   = 4;  
  NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_16Bit;     //Set counter to 16 bit resolution
  NRF_TIMER2->CC[0] = 1000;                              //Set value for TIMER2 compare register 0
  // comentout by Tamakichi
  //NRF_TIMER2->CC[1] = 0;                               //Set value for TIMER2 compare register 1
    
  // Enable interrupt on Timer 2, both for CC[0] and CC[1] compare match events
  NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) ;
  NRF_TIMER2->SHORTS =                                 // 2017/12/28 add by Tamakichi
      (TIMER_SHORTS_COMPARE0_CLEAR_Enabled << TIMER_SHORTS_COMPARE0_CLEAR_Pos);
  NVIC_EnableIRQ(TIMER2_IRQn);
    
  NRF_TIMER2->TASKS_START = 1;               // Start TIMER2
}

// Timer stop
void Adafruit_Microbit_Matrix::stopTimer(void)
{
  NRF_TIMER2->TASKS_STOP = 1; 
}  

// Timer restart
void Adafruit_Microbit_Matrix::restartTimer(void)
{
  NRF_TIMER2->TASKS_START = 1;               // Start TIMER2
}

void IRQ_MATRIX_HANDLER(void) {
  if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
  {
    NRF_TIMER2->EVENTS_COMPARE[0] = 0;         //Clear compare register 0 event 
    handle->rowHandler();
    //NRF_TIMER2->CC[0] += 1000;               // comentout by Tamakichi
  }
}

void Adafruit_Microbit_Matrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

  int16_t t;
  switch(rotation) {
   case 1:
    t = x;
    x = WIDTH  - 1 - y;
    y = t;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    t = x;
    x = y;
    y = HEIGHT - 1 - t;
    break;
  }

  // get row and col
  uint8_t pixelnum = y * 5 + x;
  uint8_t col = pixel_to_col[pixelnum] - 1;
  uint8_t row = pixel_to_row[pixelnum] - 1;
  //Serial.print("Setting row "); Serial.print(row); Serial.print(" & col "); Serial.println(col);

  if(color) 
    matrix_buffer[row][col] = 1;
  else
    matrix_buffer[row][col] = 0;
}

// 2017/12/28 add by ‚½‚Ü‹g
uint16_t Adafruit_Microbit_Matrix::getPixel(int16_t x, int16_t y) {
  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return 0;

  int16_t t;
  switch(rotation) {
   case 1:
    t = x;
    x = WIDTH  - 1 - y;
    y = t;
    break;
   case 2:
    x = WIDTH  - 1 - x;
    y = HEIGHT - 1 - y;
    break;
   case 3:
    t = x;
    x = y;
    y = HEIGHT - 1 - t;
    break;
  }
  
  // get row and col
  uint8_t pixelnum = y * 5 + x;
  uint8_t col = pixel_to_col[pixelnum] - 1;
  uint8_t row = pixel_to_row[pixelnum] - 1;
  return matrix_buffer[row][col];
}  

void Adafruit_Microbit_Matrix::clear(void) {
  fillScreen(0);
}

void Adafruit_Microbit_Matrix::fillScreen(uint16_t color) {
  for (uint8_t r=0; r<MATRIX_ROWS; r++) {
    for (uint8_t c=0; c<MATRIX_COLS; c++) {
       matrix_buffer[r][c] = color;
    }
  }
}

