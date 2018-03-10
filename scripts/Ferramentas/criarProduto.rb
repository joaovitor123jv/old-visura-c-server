#!/bin/ruby

require "./cliente.rb"

puts "******* Ferramenta para criação de produto no Visu-RA, ALPHA ******"
puts "Deseja iniciar conexão local ou com o server google ?(local/google)"
tipoConexao = gets.chomp
ip = "nada"

if tipoConexao == "local"
	ip = "localhost"
else
	ip = "35.192.214.58"
end
puts "\n\n\nIP Definido = #{ip}"


puts "Deseja logar como root ? (sim/nao)"
opcao = gets.chomp
logarRoot = false

if opcao == "sim"
	logarRoot = true
end


cliente = Cliente.new(ip, 7420)
# cliente.show()
email = ""
senha = ""

if !logarRoot
	puts "Digite seu email"
	email = gets.chomp

	puts "Digite sua senha"
	senha = gets.chomp
end

cliente.showLogs false




if logarRoot
	if cliente.connect
		puts "Cliente conectado com sucesso"
	else
		puts "Infelizmente ocorreu um erro ao tentar se conectar ao servidor"
		exit 1
	end	
else
	if cliente.connect2(email, senha)
		puts "Cliente conectado com sucesso"
	else
		puts "Infelizmente ocorreu um erro ao tentar se conectar ao servidor"
		exit 1
	end
end

puts "\n\nDigite o ID do produto que deseja criar"
idProduto = gets.chomp().tr(' ','_')

puts "\n\nDigite a duracao do produto 0→90"
duracao = gets.chomp()

puts "\n\nDigite o nome do produto"
nomeProduto = gets.chomp().tr(' ','_')

puts "\n\nDigite o tipo do produto (RA/RV/ambos)"
tipoProduto = gets.chomp
if tipoProduto == "RA"
	tipoProduto = "1"
elsif tipoProduto == "RV"
	tipoProduto = "2"
else
	tipoProduto = "3"
end

puts "\n\nDigite a categoria do produto (dois caracteres)"
categoriaProduto = gets.chomp

puts "\n\nDigite a descricao do produto"
descricaoProduto = gets.chomp.tr(' ','_')

idContratante = ""

if logarRoot
	puts "Digite o ID do contratante, ROOT"
	idContratante = gets.chomp
end

puts "Dados até agora:"
puts "Descricao de produto = #{descricaoProduto}"
puts "Nome de produto = #{nomeProduto}"
puts "Tipo de produto = #{tipoProduto}"
puts "Duracao do produto = #{duracao}"
puts "ID de produto = #{idProduto}"
if logarRoot
	puts "ID de contratante = #{idContratante}"
end


puts "Formatando mensagem"
mensagem = ""
if logarRoot
	mensagem = "#{cliente.getChaveAplicacao} 2 + #{cliente.getChaveSeguranca} #{idContratante} #{idProduto} #{duracao} #{nomeProduto} #{tipoProduto} #{descricaoProduto}"
	#LEMBRE QUE O PRODUTO ADICIONADO POR ROOT NÂO POSSUI DESCRICAO
else
	mensagem = "#{cliente.getChaveAplicacao} 2 + #{idProduto} #{duracao} #{nomeProduto} #{tipoProduto} #{categoriaProduto} #{descricaoProduto}"
end

puts "Mensagem formatada = |#{mensagem}|"



resposta = cliente.send(mensagem)
if resposta == "OK\0"
	puts "Produto #{idProduto} adicionado ao banco de dados com sucesso :D"
else
	puts "Infelizmente não foi possível adicionar o produto ao banco de dados :("
end

cliente.encerrar

# puts "Uso interno, somente"
# puts "\n\n\n Deseja criar o produto em qual empresa (digite o login da empresa) ? (para ver a lista de empresas disponíveis digite \"list\")"
# empresa = gets.chomp()

# if empresa == "list"
# 	puts "Você entao deseja ver a lista de empresas disponíveis ?"
# end

# puts "Empresa entendida = #{empresa}"

# puts "Digite o ID do produto que você quer criar"