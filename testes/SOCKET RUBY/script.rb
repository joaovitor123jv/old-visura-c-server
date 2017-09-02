#!/bin/ruby
#
require 'socket'

hostname = '127.0.0.1'
port = 7420

s = TCPSocket.open(hostname, port)


retorno = "APP 1 interface@servidor senatauri"
s.puts retorno.chop
puts "\t" + retorno

retorno =  s.recv(140)
puts retorno

retorno = "APP 4 Q * abcdefghij\0"
puts "\t" + retorno
s.puts retorno.chop

retorno = s.recv(140)
puts retorno.chop

retorno = "APP sair"
s.puts retorno.chop
puts "\t" + retorno

retorno =  s.recv(140)
puts retorno

#while line = s.gets
#	puts line.chop
#end

s.close
