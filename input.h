#ifndef INPUT_H
#define INPUT_H

#define DOWN 1
#define UP 0

void mouse_handler (int x, int y);
void key_down (unsigned char key, int x, int y);
void key_up (unsigned char key, int x, int y);
void spec_key_down (int key, int x, int y);
void spec_key_up (int key, int x, int y);
void input_update (void);

#endif
