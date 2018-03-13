#!/usr/bin/ruby
require './cliente.rb'

puts "Digite o nome da cidade"
nomeCidade = gets.chomp

puts "Digite a sigle do estado"
nomeEstado = gets.chomp

nomeCidade = nomeCidade.tr(' ', '_')


# cliente = Cliente.new("localhost", PORTA_INTERFACE)
cliente = Cliente.new(IP_GOOGLE, PORTA_INTERFACE)

cliente.connect

mensagem = "APP 2 { "+nomeEstado+" "+nomeCidade

retorno = cliente.send(mensagem)

if retorno == "OK\0"
	puts "Cidade adicionada com sucesso"

else
	puts "Falha ao adicionar cidade"
end