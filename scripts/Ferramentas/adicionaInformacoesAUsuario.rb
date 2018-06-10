#!/usr/bin/ruby

require './cliente.rb'

puts "Digite o login do usuario"
login = gets.chomp

puts "Digite a senha do usuario"
senha = gets.chomp

#cliente = Cliente.new(IP_GOOGLE, PORTA_INTERFACE)
cliente = Cliente.new("localhost", PORTA_INTERFACE)


if cliente.connect2(login, senha)

	puts "OK, usuario está conectado"

	puts "Digite um nome pra esse usuario"
	nome = gets.chomp

	puts "Digite um sobrenome pra esse usuario"
	sobrenome = gets.chomp

	puts "Digite a cidade onde esse usuario mora"
	cidade = gets.chomp

	puts "Qual o sexo desse usuário ?\n1 = Masculino\n2 = Feminino\n3 = Outro"
	sexo = gets.chomp

	puts "Qual é a data de nascimento desse usuário ?"
	dataNascimento = gets.chomp








	mensagem = "APP 2 1 1 7 "+login+" 9 "+senha+" "+sexo+" "+dataNascimento

	retorno = cliente.send(mensagem)

	if retorno == "OK\0"
		puts "Usuário adicionado com sucesso"
	else
		puts "Falha ao adicionar quantidade de visualizacoes"
	end

	puts "Script encerrado"

	# cliente.encerrar
end

cliente.encerrar