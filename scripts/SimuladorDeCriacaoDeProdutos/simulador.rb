#!/usr/bin/ruby
#

require 'mysql2'

@tempo_entre_insercoes = 2

@database_password = "essaSenhaEsoPraInteFaCeSaBeRPorQuESiM"
@database_host = "localhost"
@database_name = "teste"
@database_username = "interface"

$client = Mysql2::Client.new(:host => @database_host, :username => @database_username, :password => @database_password, :database => @database_name)

def insereProduto(idproduto, emailContratante, duracao)
	sleep @tempo_entre_insercoes
	puts "Adicionando produto #{idproduto} a empresa #{emailContratante}"
	$client.query("INSERT INTO produto(contratante_idcontratante,duracao,idproduto) SELECT contratante.idcontratante,#{duracao},\'#{idproduto}\' FROM contratante WHERE contratante.email=\'#{emailContratante}\';")
end


puts "Adicionando produto 'abcdefghij'"
insereProduto 'abcdefghij', 'coca-cola@coca.com', 30

puts "Adicionando produto 'jJjahHhgaG'"
insereProduto 'jJjahHhgaG', 'coca-cola@coca.com', 30

puts "Adicionando produto 'alksjqnjdk'"
insereProduto 'alksjqnjdk', 'susheria@comida.com', 30

puts "Adicionando produto 'epoaiskqjn'"
insereProduto 'epoaiskqjn', 'coca-cola@coca.com', 30

puts "Adicionando produto 'ajruqidhxc'"
insereProduto 'ajruqidhxc', 'coca-cola@coca.com', 30

puts "Adicionando produto 'ahgr0xi1g8'"
insereProduto 'ahgr0xi1g8', 'susheria@comida.com', 30

puts "Adicionando produto 'ashiwoiv09'"
insereProduto 'ashiwoiv09', 'coca-cola@coca.com', 30

puts "Adicionando produto 'aaaaabbbcc'"
insereProduto 'aaaaabbbcc', 'coca-cola@coca.com', 30

