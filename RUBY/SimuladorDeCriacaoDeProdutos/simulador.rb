#!/usr/bin/ruby
#

require 'mysql2'


$client = Mysql2::Client.new(:host => "127.0.0.1", :username => "interface", :password => "senha", :database => "teste")

sleep 7
puts "Adicionando produto 'abcdefghij'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'abcdefghij\' FROM contratante WHERE contratante.email = \'coca-cola@coca.com\';")

sleep 2
puts "Adicionando produto 'jJjahHhgaG'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'jJjahHhgaG\' FROM contratante WHERE contratante.email = \'coca-cola@coca.com\';")


sleep 1
puts "Adicionando produto '99aah9811h'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'99aah9811h\' FROM contratante WHERE contratante.email = \'coca-cola@coca.com\';")

sleep 5
puts "Adicionando produto 'sushiahHHj'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'sushiahHHj\' FROM contratante WHERE contratante.email = \'susheria@comida.com\';")
sleep 3
puts "Adicionando produto 'onigiriOOo'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'onigiriOOo\' FROM contratante WHERE contratante.email = \'susheria@comida.com\';")
sleep 1
puts "Adicionando produto '11827gaubw'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'11827gaubw\' FROM contratante WHERE contratante.email = \'coca-cola@coca.com\';")
sleep 1
puts "Adicionando produto 'hauebqiIUa'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'hauebqiIUa\' FROM contratante WHERE contratante.email = \'coca-cola@coca.com\';")
sleep 9
puts "Adicionando produto 'IiIiIiIiIi'"
$client.query("INSERT INTO produto(contratante_idcontratante, duracao, idproduto) SELECT contratante.idcontratante, 30, \'IiIiIiIiIi\' FROM contratante WHERE contratante.email = \'coca-cola@coca.com\';")


#
#def monitor(idproduto)
#	client = Mysql2::Client.new(:host => "127.0.0.1", :username => "interface", :password => "senha", :database => "teste")
#
#	result = client.query("SELECT visualizacoes, visualizacaoanom from produto WHERE idproduto='#{idproduto}';")
#	puts result.count
#	visualAnterior = 0
#	visualAtual = 0
#	visualAnomAtual = 0
#	visualAnomAnterior = 0
#
#
#	result.each do |row|
#		visualAnterior = row["visualizacoes"]
#		visualAnomAnterior = row["visualizacaoanom"]
#	end
#
#	sleep 1
#	result = client.query("SELECT visualizacoes, visualizacaoanom from produto WHERE idproduto='#{idproduto}';")
#
#	result.each do |row|
#		visualAnomAtual = row["visualizacaoanom"]
#		visualAtual = row["visualizacoes"]
#	end
#
#
#	query = "INSERT INTO LogProduto(visualAnomAnterior, visualAnomAtual, visualAnterior, visualAtual, produto_idproduto) VALUES(#{visualAnomAnterior}, #{visualAnomAtual}, #{visualAnterior}, #{visualAtual}, '#{idproduto}');"
#	result = client.query(query)
#	puts "Log Efetuado : #{ idproduto}"
#	result.each do |row|
#		puts row
#	end
#
#
#end
#
#
#
#
#
#
#
#
#while(true)
#	sleep 3
#	result = $client.query("SELECT idproduto from produto;")
#
##	arr = Array.new(result.count)
##	i = -1
#
#	result.each do |row|
##		monitor(row["idproduto"])
##		i = i + 1
#
#		puts row["idproduto"]
##		arr[i] = Thread.new{monitor(row["idproduto"])}
#		Thread.new{ monitor(row["idproduto"]) }
##		t1.join()
#	end
###	i = 0
##	while(i < result.count)
##		arr[i].join
##		i = i + 1
##	end
#
#
#end
#
