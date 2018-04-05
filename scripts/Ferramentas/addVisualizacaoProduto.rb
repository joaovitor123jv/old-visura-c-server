#!/usr/bin/ruby

require './cliente.rb'

puts "Digite o id do produto"
idProduto = gets.chomp

puts "Digite a quantidade de visualizações"
quantidade = gets.chomp

cliente = Cliente.new("localhost", PORTA_INTERFACE)
#cliente = Cliente.new(IP_GOOGLE, PORTA_INTERFACE)

cliente.connect

mensagem = "APP 2 2 "+idProduto + " "+ quantidade

retorno = cliente.send(mensagem)

if retorno == "OK\0"
	puts "Quantidade de visualizações adicionada com sucesso"
else
	puts "Falha ao adicionar quantidade de visualizacoes"
end

cliente.encerrar
