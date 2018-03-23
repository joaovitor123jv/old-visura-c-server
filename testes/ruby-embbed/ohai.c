#include "ruby.h"
#include <stdio.h>

int main() 
{
  VALUE *ohai;


  printf("OHAI from C\n");

  ruby_init();
  rb_require("./ohai.rb");
  rb_eval_string("Ohai.new");
  int i = 0;
  for(i = 0; i<1000; i++)
  {
	  printf("OHAI from C\n");
  }
  exit(0);
}
