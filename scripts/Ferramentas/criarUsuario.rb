#!/usr/bin/ruby

require './cliente.rb'

puts "Digite um login pra esse usuario"
login = gets.chomp

puts "Digite uma senha pra esse usuario"
senha = gets.chomp

puts "Qual o sexo desse usuário ?\n1 = Masculino\n2 = Feminino\n3 = Outro"
sexo = gets.chomp

puts "Qual é a data de nascimento desse usuário ?"
dataNascimento = gets.chomp



cliente = Cliente.new("localhost", PORTA_INTERFACE)
#cliente = Cliente.new(IP_GOOGLE, PORTA_INTERFACE)

cliente.connect

mensagem = "APP 2 1 1 7 "+login+" 9 "+senha+" "+sexo+" "+dataNascimento

retorno = cliente.send(mensagem)

if retorno == "OK\0"
	puts "Usuário adicionado com sucesso"
else
	puts "Falha ao adicionar quantidade de visualizacoes"
end

puts "Script encerrado"

cliente.encerrar
