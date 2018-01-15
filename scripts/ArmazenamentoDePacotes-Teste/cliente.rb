#!/usr/bin/ruby

require "socket"

#hostname = "localhost"
hostname = "192.168.1.62"
port = 80

server = TCPSocket.open(hostname, port)

#conteudo = "Esse eh o conteudo dez\nteste\ndoarquivo que eu quero saber se isso funciona mesmo pô \nmuahaha\rlalala\ntralalalala"
conteudo = File.open("PrimeiraTemp.zip", "r")

puts "Tamanho hipotetico do arquivo: #{conteudo.size} Bytes"

#line = gets
line = "SEND primeiraTempAnsatsuKyoushitsu.zip"
server.puts line

while line = conteudo.read(1024*1024*100)
	server.write(line)
end

server.close
server = TCPSocket.open(hostname, port)

server.puts "CHCK primeiraTempAnsatsuKyoushitsu.zip #{conteudo.size}"
retorno = server.read(100)
puts "Retorno: #{retorno}"

#while line = server.read(101)
#	puts line
#end

#while line = gets
#	server.puts line
#	puts "Linha enviada = "+ line
#end
