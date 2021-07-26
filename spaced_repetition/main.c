#include <ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define CARD_FLIP_SECS 0.025
#define MILLION 1000000
#define MAX_CARD_TEXT_LEN 99

struct CardText {
  char *q;
  char *a;
};

struct CardView {
  WINDOW *win;
  struct CardText *text;
  int textx;
  int texty;
  int is_facing_up;
};

void flip_card(struct CardView *cv) {
  int starting_width = getmaxx(cv->win);
  int shrinking = 1;
  int new_width;
  int done = 0;
  while (!done) {
    erase();
    werase(cv->win);
    refresh();
    int width = getmaxx(cv->win);
    new_width = width - 2;
    if (width < 2) {
      shrinking = 0;
      cv->is_facing_up = !cv->is_facing_up;
    }
    new_width = width + (shrinking ? -2 : 2);
    wresize(cv->win, getmaxy(cv->win), new_width);
    mvwin(cv->win, getbegy(cv->win), getbegx(cv->win) + (shrinking ? 1 : -1));
    box(cv->win, 0, 0);
    mvwprintw(cv->win, cv->texty, cv->textx, (cv->is_facing_up ? cv->text->q : cv->text->a));
    wrefresh(cv->win);

    if (new_width == starting_width)
      done = 1;

    usleep(CARD_FLIP_SECS * MILLION);
  }
}

void init() {
  initscr();
  noecho();
  keypad(stdscr, 1);
  refresh();
}

int main () {
  init();
  int height = 25;
  int width = 35;
  int inity = 0;
  int initx = 0;
  WINDOW *card_win = newwin(height, width, inity, initx);
  box(card_win, 0, 0);

  struct CardText texts[] = {
    {
      .q = "q 1",
      .a = "a 1",
    },
    {
      .q = "q 2",
      .a = "a 2",
    },
    {
      .q = "q 3",
      .a = "a 3",
    },
  };

  int curr_text_idx = 0;

  struct CardView cv = {
    .win = card_win,
    .text = &texts[curr_text_idx],
    .textx = 2,
    .texty = 3,
    .is_facing_up = 1,
  };

  mvwprintw(cv.win, cv.texty, cv.textx, (cv.is_facing_up ? cv.text->q : cv.text->a));
  wrefresh(cv.win);
  int ch;
  while ((ch = getch()) != 'q') {
    switch(ch) {
      case 'f':
        flip_card(&cv);
        break;
      case 'n':
        curr_text_idx = (curr_text_idx == 2) ? 0 : ++curr_text_idx;
        cv.text = &texts[curr_text_idx];
        mvwprintw(cv.win, cv.texty, cv.textx, (cv.is_facing_up ? cv.text->q : cv.text->a));
        wrefresh(cv.win);
        break;
      case 'p':
        curr_text_idx = (curr_text_idx == 0) ? 2 : --curr_text_idx;
        cv.text = &texts[curr_text_idx];
        mvwprintw(cv.win, cv.texty, cv.textx, (cv.is_facing_up ? cv.text->q : cv.text->a));
        wrefresh(cv.win);
        break;
    }
  }
  getch();
  endwin();
  return 0;
}
