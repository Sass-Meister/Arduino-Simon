const int SPEAKER = 11;

const int RED_LED = 2;
const int GREEN_LED = 3;
const int BLUE_LED = 4;
const int YELLOW_LED = 5;

const int RED_BUTTON = 6;
const int GREEN_BUTTON = 7;
const int BLUE_BUTTON = 8;
const int YELLOW_BUTTON = 9;

const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;
const int YELLOW = 3;

const int BAD = -1;

int skill;

int seq[32];

void setup()
{

  setAll(false); // Pins start out at 0, turing the LEDs on

  pinMode(SPEAKER, OUTPUT);

  pinMode(BLUE_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(RED_LED, OUTPUT);

  pinMode(YELLOW_BUTTON, INPUT_PULLUP);
  pinMode(YELLOW_LED, OUTPUT);

  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
}

void loop()
{
  int skill = getSkill(attract());
  delay(500); //to give the user time to remove thier finger from the button

  fillSeq();

  int last = playSimon(skill);

  if (last != skill)
  {
    lose(seq[last]);
  }
  else
  {
    win(seq[skill - 1]);
  }
}

void ledOn(int color)
{
  if (color > 3 || color < 0)
    return;

  digitalWrite(color + 2, 0);
}

void ledOff(int color)
{
  if (color > 3 || color < 0)
    return;

  digitalWrite(color + 2, 1);
}

void setAll(boolean on) // Sets all the LEDs to a desired state (true = on and false = off)
{
  if (on)
  {
    ledOn(BLUE);
    ledOn(RED);
    ledOn(YELLOW);
    ledOn(GREEN);
  }

  else
  {
    ledOff(BLUE);
    ledOff(RED);
    ledOff(YELLOW);
    ledOff(GREEN);
  }
}

void colorTone(int color) // -1 == off, -2 loser
{
  if (color == BAD)
  {
    noTone(SPEAKER);
  }

  else if (color == -2)
  {
    tone(SPEAKER, 42);
  }

  else
  {
    switch (color) { // the fanciest of if statements
      case RED: //red
        tone(SPEAKER, 310);
        break;

      case BLUE:
        tone(SPEAKER, 209);
        break;

      case YELLOW:
        tone(SPEAKER, 252);
        break;

      case GREEN:
        tone(SPEAKER, 415);
        break;
    }
  }
}

void win(int color)
{
  setAll(false);

  for (int i = 1; i <= 6; i++)
  {
    ledOn(color);
    colorTone(color);
    delay(70);

    ledOff(color);
    delay(20);
  }
  colorTone(BAD);
}

void lose(int color)
{
  setAll(false);

  colorTone(-2);
  ledOn(color);
  delay(1500);

  colorTone(BAD);
  ledOff(color);
}

int getButton()
{
  int b = BAD;

  if (!digitalRead(GREEN_BUTTON))
    b = GREEN;

  else if (!digitalRead(RED_BUTTON))
    b = RED;

  else if (!digitalRead(BLUE_BUTTON))
    b = BLUE;

  else if (!digitalRead(YELLOW_BUTTON))
    b = YELLOW;

  delay(50); // to preent bouncing and multiple inputs

  return b;
}

int attract()
{
  int skill;
  long oldTime = millis();
  long newTime;

  const int SPEED = 500;

  while ((skill = getButton()) == BAD)
  {
    newTime = millis();

    if (newTime > (oldTime + (SPEED * 8)))
    {
      oldTime = newTime;
    }
    else if (newTime > (oldTime + 100 + (SPEED * 7)))
    {
      ledOff(YELLOW);
    }
    else if (newTime > (oldTime + 100 + (SPEED * 6)))
    {
      ledOff(GREEN);
    }
    else if (newTime > (oldTime + 100 + (SPEED * 5)))
    {
      ledOff(RED);
    }
    else if (newTime > (oldTime + 100 + (SPEED * 4)))
    {
      ledOff(BLUE);
    }
    else if (newTime > (oldTime + 100 + (SPEED * 3)))
    {
      ledOn(YELLOW);
    }
    else if (newTime > (oldTime + 100 + (SPEED * 2)))
    {
      ledOn(GREEN);
    }
    else if (newTime > (oldTime + 100 + (SPEED * 1)))
    {
      ledOn(RED);
    }
    else if (newTime > (oldTime + 100))
    {
      ledOn(BLUE);
    }
  }
  setAll(false);
  return skill;
}

boolean timedGetButton(int color)
{
  int button = BAD;
  long start = millis();

  while (((millis() - start) < 3000) && ((button = getButton()) == BAD));

  while (getButton() == color)
  {
    ledOn(color);
    colorTone(color);
  }
  colorTone(BAD);
  ledOff(color);

  delay(100);
  return button == color;
}

void ledPlay(int color, int seq)
{
  ledOn(color);
  colorTone(color);

  if (seq >= 14)
  {
    delay(220);
  }
  else if (seq >= 6)
  {
    delay(320);
  }
  else if (seq >= 1)
  {
    delay(420); // blaze 4 dayz
  }

  colorTone(BAD);
  ledOff(color);
  delay(50);
}

void fillSeq()
{
  randomSeed(millis());

  for (int i = 0; i < 32; i++)
  {
    seq[i] = random(4);
  }
}

void playSeq(int amount)
{
  for (int i = 0; i < amount; i++)
  {
    ledPlay(seq[i], amount);
  }
}

int userSeq(int amount)
{
  int i = 0;

  while (i < amount)
  {
    boolean pass = timedGetButton(seq[i]);

    if (!pass)
    {
      break;
    }
    i++;
  }
  while (getButton() != BAD); // to prevent the user from holding down a button after the sequence is over. May not be needed in the future or just need a delay.
  return i;
}

int playSimon(int rounds)
{
  int buttons = 0;
  rounds = rounds > 32 ? 32 : rounds; // never bigger then 32

  for (int i = 0; i <= rounds; i++)
  {
    playSeq(i);

    if ((buttons = userSeq(i)) != i)
    {
      break;
    }
    delay(800);
  }

  return buttons;
}

int getSkill(int color)
{
  switch (color) {
    case GREEN:
      return 8;
    case BLUE:
      return 16;
    case YELLOW:
      return 24;
    case RED:
      return 32;
  }
}

void test() // tests buttons and LEDs
{
  digitalWrite(BLUE_LED, digitalRead(BLUE_BUTTON));
  digitalWrite(RED_LED, digitalRead(RED_BUTTON));
  digitalWrite(GREEN_LED, digitalRead(GREEN_BUTTON));
  digitalWrite(YELLOW_LED, digitalRead(YELLOW_BUTTON));

  colorTone(!digitalRead(BLUE_BUTTON) ? BLUE : -3);
  colorTone(!digitalRead(RED_BUTTON) ? RED : -3);
  colorTone(!digitalRead(YELLOW_BUTTON) ? YELLOW : -3);
  colorTone(!digitalRead(GREEN_BUTTON) ? GREEN : -3);
  colorTone(digitalRead(GREEN_BUTTON) && digitalRead(YELLOW_BUTTON) && digitalRead(RED_BUTTON) && digitalRead(BLUE_BUTTON) ? -1 : -3);
}
