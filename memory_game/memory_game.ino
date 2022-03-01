int LED_RED = 12;
int LED_YELLOW = 11;
int LED_GREEN = 10;
int LED_WHITE = 9;

int game_leds[] = {LED_RED, LED_YELLOW, LED_GREEN, LED_WHITE};
int game_leds_length = sizeof game_leds / sizeof game_leds[0];

int round_secret[10];
int round_secret_length = sizeof round_secret / sizeof round_secret[0];

int game_round = -1;
int round_turn = -1;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  randomSeed(analogRead(0));
}

void loop() {
  delay(10);

  if (game_round >= round_secret_length) {
    game_victory();
    return;
  }

  if (game_round < 0) {
    game_start();
    return;
  }

  if (round_turn < 0) {
    round_start();
    return;
  }

  int led = read_pressed_led();

  if (led < 0) return;

  Serial.println(led);

  if (led != round_secret[round_turn]) {
    game_over();
    return;
  }

  if (round_turn == game_round) {
    game_round_victory();
    return;
  }

  round_turn++;
}

int read_pressed_led() {
  for (int i = 0; i < game_leds_length; i++) {
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
  game_round = 0;
  round_turn = -1;
}

void game_victory() {
  set_leds_output_mode();

  show_victory_animation();

  game_round = -1;
}

void round_start() {
  set_leds_output_mode();

  show_reset_animation();

  set_new_round_secret();

  show_round_secret_animation();

  round_turn = 0;

  set_leds_input_mode();
}

void game_over() {
  Serial.println("game over!");
  set_leds_output_mode();

  show_reset_animation();

  round_turn = -1;
  game_round = -1;
}

void game_round_victory() {
  Serial.println("round win!");
  set_leds_output_mode();

  show_round_victory_animation();

  round_turn = -1;
  game_round++;
}

void set_new_round_secret() {
  for (int i = 0; i < round_secret_length; i++) {
    int rand_index = random(0, game_leds_length);
    round_secret[i] = game_leds[rand_index];
  }
}

void show_reset_animation() {
  for (int i = 0; i < game_leds_length; i++) {
    digitalWrite(game_leds[i], HIGH);
  }

  delay(1000);

  for (int i = 0; i < game_leds_length; i++) {
    digitalWrite(game_leds[i], LOW);
  }

  delay(1000);
}

void show_round_victory_animation() {
  for (int j = 0; j < 2; j++) {
    for (int i = 0; i < game_leds_length; i++) {
      digitalWrite(game_leds[i], HIGH);
      delay(100);
      digitalWrite(game_leds[i], LOW);
    }
  }
}

void show_round_secret_animation() {
  for (int i = 0; i < game_round + 1; i++) {
    digitalWrite(round_secret[i], HIGH);
    delay(500);
    digitalWrite(round_secret[i], LOW);
    delay(50);
  }
}

void show_victory_animation() {
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < game_leds_length; i++) {
      digitalWrite(game_leds[i], HIGH);
      delay(100);
      digitalWrite(game_leds[i], LOW);
    }
    for (int i = 0; i < game_leds_length; i++) {
      digitalWrite(game_leds[game_leds_length-i], HIGH);
      delay(100);
      digitalWrite(game_leds[game_leds_length-i], LOW);
    }
  }
}

void set_leds_output_mode() {
  for (int i = 0; i < game_leds_length; i++) {
    pinMode(game_leds[i], OUTPUT);
  }
}

void set_leds_input_mode() {
  for (int i = 0; i < game_leds_length; i++) {
    pinMode(game_leds[i], INPUT);
  }
}
