int LED_RED = 12;
int LED_YELLOW = 11;
int LED_GREEN = 10;
int LED_WHITE = 9;

int game_leds[] = {LED_RED, LED_YELLOW, LED_GREEN, LED_WHITE};
int game_leds_count = sizeof game_leds / sizeof game_leds[0];

int order[] = {LED_RED, LED_WHITE, LED_YELLOW, LED_GREEN};
int order_count = sizeof order / sizeof order[0];

int game_turn = -1;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  randomSeed(analogRead(0));
}

void loop() {
  delay(10);

  if (game_turn < 0) {
    game_start();
  }

  int led = read_pressed_led();

  if (led < 0) return;

  Serial.println(led);

  if (led != order[game_turn]) {
    Serial.println("game over!");
    game_over();
    return;
  }

  game_turn++;

  if (game_turn == game_leds_count) {
    Serial.println("win!");
    game_victory();
  }
}

int read_pressed_led() {
  for (int i = 0; i < game_leds_count; i++) {
    int value = digitalRead(game_leds[i]);
    if (value == HIGH) {
      while (digitalRead(game_leds[i])) {
        delay(1);
      }
      return game_leds[i];
    }
  }
  return -1;
}

void game_start() {
  set_leds_output_mode();

  show_reset_animation();

  set_new_order();

  show_order_animation();

  set_leds_input_mode();

  game_turn = 0;
}

void game_over() {
  set_leds_output_mode();

  show_reset_animation();

  game_turn = -1;
}

void game_victory() {
  set_leds_output_mode();

  show_victory_animation();

  game_turn = -1;
}

void set_new_order() {
  for (int i = 0; i < order_count; i++) {
    int rand_index = random(0, game_leds_count);
    order[i] = game_leds[rand_index];
  }
}

void show_reset_animation() {
  for (int i = 0; i < game_leds_count; i++) {
    digitalWrite(game_leds[i], HIGH);
  }

  delay(1000);

  for (int i = 0; i < game_leds_count; i++) {
    digitalWrite(game_leds[i], LOW);
  }

  delay(1000);
}

void show_victory_animation() {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < game_leds_count; i++) {
      digitalWrite(game_leds[i], HIGH);
      delay(100);
      digitalWrite(game_leds[i], LOW);
    }
  }
}

void show_order_animation() {
  for (int i = 0; i < order_count; i++) {
    digitalWrite(order[i], HIGH);
    delay(500);
    digitalWrite(order[i], LOW);
    delay(50);
  }
}

void set_leds_output_mode() {
  for (int i = 0; i < game_leds_count; i++) {
    pinMode(game_leds[i], OUTPUT);
  }
}

void set_leds_input_mode() {
  for (int i = 0; i < game_leds_count; i++) {
    pinMode(game_leds[i], INPUT);
  }
}
