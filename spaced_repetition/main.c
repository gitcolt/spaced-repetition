#include <ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define CARD_FLIP_SECS 0.025
#define MILLION 1000000
#define MAX_CARD_TEXT_LEN 99

struct CardView {
  WINDOW *card;
  char text_q[MAX_CARD_TEXT_LEN];
  char text_a[MAX_CARD_TEXT_LEN];
  char **curr_text;
  int textx;
  int texty;
};

void switch_text(struct CardView *c) {
  char *tmp;
  if (*c->curr_text == c->text_q) {
    tmp = c->text_a;
  } else {
    tmp = c->text_q;
  }
  *c->curr_text = tmp;
}

void flip_card(struct CardView *c) {
  int starting_width = getmaxx(c->card);
  int shrinking = 1;
  int new_width;
  int done = 0;
  while (!done) {
    erase();
    werase(c->card);
    refresh();
    int width = getmaxx(c->card);
    new_width = width - 2;
    if (width < 2) {
      shrinking = 0;
      switch_text(c);
    }
    new_width = width + (shrinking ? -2 : 2);
    wresize(c->card, getmaxy(c->card), new_width);
    mvwin(c->card, getbegy(c->card), getbegx(c->card) + (shrinking ? 1 : -1));
    box(c->card, 0, 0);
    mvwprintw(c->card, c->texty, c->textx, *(c->curr_text));
    wrefresh(c->card);

    if (new_width == starting_width)
      done = 1;

    usleep(CARD_FLIP_SECS * MILLION);
  }
}

int main () {
  initscr();
  noecho();
  keypad(stdscr, 1);
  refresh();
  int height = 25;
  int width = 35;
  int inity = 0;
  int initx = 0;
  WINDOW *card = newwin(height, width, inity, initx);
  box(card, 0, 0);

  struct CardView c = {
    .card = card,
    .text_q = "QQQQQQQQ",
    .text_a = "AAAAAAAA",
    .texty = 2,
    .textx = 3,
  };
  char *tmp = c.text_q;
  c.curr_text = &tmp;

  wrefresh(card);
  int ch;
  while ((ch = getch()) != 'q') {
    switch(ch) {
      case 'f':
        flip_card(&c);
        break;
    }
  }
  getch();
  endwin();
  return 0;
}
