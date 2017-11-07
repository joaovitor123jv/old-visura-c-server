#!/bin/ruby
#
require 'mysql2'


#$client = Mysql2::Client.new(:host => "127.0.0.1", :username => "interface", :password => "senha", :database => "teste")

$client = Mysql2::Client.new(:host => "localhost", :username => "interface", :password => "essaSenhaEsoPraInteFaCeSaBeRPorQuESiM", :database => "teste")

#def monitor(idproduto)
#	cliente = Mysql2::Client.new(:host => "127.0.0.1", :username => "interface", :password => "senha", :database => "teste")
#	while(true)
#		resultado = cliente.query("SELECT visualizacoes from produto WHERE idproduto='#{idproduto}'")
#		puts " Visualizacoes anonimas de #{idproduto}  = "
##	puts resultado.each_hash{ |h| puts h["visualizacaoanom"] }
#		qtdAnterior =0
#		resultado.each do |row|
#			qtdAnterior= row["visualizacoes"]
#		end
#		puts "#{ qtdAnterior}"
#
#		sleep 1
#
#		resultado = cliente.query("SELECT visualizacoes from produto WHERE idproduto='#{idproduto}'")
#		qtdAtual = 0
#		resultado.each do |row|
#			qtdAtual = row["visualizacaoanom"]
#		end
#
#		diferenca = qtdAtual - qtdAnterior
#		puts "Diferenca entre visualizacoes = #{diferenca}"
#		resultado = cliente.query("INSERT INTO LogProduto(visualAnomAnterior,visualAnomAtual,visualAnterior,visualAtual,produto_idproduto) VALUES(#{qtdAnterior}, #{qtdAtual}, 0, 0, '#{ idproduto}';")
#	end
#end



def monitor(idproduto)
	client = Mysql2::Client.new(:host => "127.0.0.1", :username => "interface", :password => "senha", :database => "teste")

	result = client.query("SELECT visualizacoes, visualizacaoanom from produto WHERE idproduto='#{idproduto}';")
	puts result.count
	visualAnterior = 0
	visualAtual = 0
	visualAnomAtual = 0
	visualAnomAnterior = 0


	result.each do |row|
		visualAnterior = row["visualizacoes"]
		visualAnomAnterior = row["visualizacaoanom"]
	end

	sleep 1
	result = client.query("SELECT visualizacoes, visualizacaoanom from produto WHERE idproduto='#{idproduto}';")

	result.each do |row|
		visualAnomAtual = row["visualizacaoanom"]
		visualAtual = row["visualizacoes"]
	end


	query = "INSERT INTO LogProduto(visualAnomAnterior, visualAnomAtual, visualAnterior, visualAtual, produto_idproduto) VALUES(#{visualAnomAnterior}, #{visualAnomAtual}, #{visualAnterior}, #{visualAtual}, '#{idproduto}');"
	result = client.query(query)
	puts "Log Efetuado : #{ idproduto}"
	result.each do |row|
		puts row
	end


end








while(true)
	sleep 3
	result = $client.query("SELECT idproduto from produto;")

#	arr = Array.new(result.count)
#	i = -1

	result.each do |row|
#		monitor(row["idproduto"])
#		i = i + 1

		puts row["idproduto"]
#		arr[i] = Thread.new{monitor(row["idproduto"])}
		Thread.new{ monitor(row["idproduto"]) }
#		t1.join()
	end
##	i = 0
#	while(i < result.count)
#		arr[i].join
#		i = i + 1
#	end


end




#result = client.query("SELECT visualizacoes from produto WHERE produto.idproduto='cocacomnos';")
#
#puts result

#result.each do |row|
#	puts row["visualizacoes"]
#end

#result.each do |row|
#	puts row
#	puts "\n"
#	puts row["idproduto"]
#	puts "\n\n"
#end
#	
