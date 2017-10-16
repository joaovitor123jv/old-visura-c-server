#!/bin/ruby

require 'socket'

$porta = 7420
$IP = "127.0.0.1"


puts "Deseja iniciar com as configurações padrão ? (S/N)"

resposta = gets.chomp

if(resposta == "S" || resposta == "s") then
	puts "Valores padrão carregados."
else
	puts "Digite a porta que deseja \"escutar\""
	porta = gets.chomp
	$porta = Integer(porta)
	puts "OK, Digite o IP, ou DNS do destino"
	$IP = gets.chomp
end

$server = TCPSocket.open($IP, $porta)
puts "Cliente conectado!"

while true do
	puts "→Digite a mensagem que deseja enviar ao servidor"
	mensagem = gets.chomp
	puts "→ Mensagem enviada = #{mensagem}"
	$server.puts "#{mensagem.chomp}\0"
	resposta = $server.recvfrom(1024)
	puts "\tResposta do servidor → #{resposta}"
end
