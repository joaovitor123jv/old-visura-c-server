# git clone git://gist.github.com/933632.git /tmp/ohai
# cd /tmp/ohai
# make
# ./ohai

ohai: ohai.c ohai.rb
	gcc  ohai.c -o ohai `pkg-config --cflags --libs ruby-2.3`
