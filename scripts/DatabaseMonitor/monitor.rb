#!/bin/ruby

require 'mysql2'

@tempo_entre_logs = 15

@database_password = "essaSenhaEsoPraInteFaCeSaBeRPorQuESiM"
@database_host = "localhost"
@database_name = "teste"
@database_username = "interface"

$client = Mysql2::Client.new(:host => @database_host, :username => @database_username, :password => @database_password, :database => @database_name)

def monitor(idproduto)
	client = Mysql2::Client.new(:host => @database_host, :username => @database_username, :password => @database_password, :database => @database_name)
	visualAtual = 0
	visualAnomAtual = 0
	result = client.query("SELECT visualizacoes, visualizacaoanom from produto WHERE idproduto='#{idproduto}';")
	result.each do |row|
		visualAnomAtual = row["visualizacaoanom"]
		visualAtual = row["visualizacoes"]
	end
	query = "INSERT INTO LogProduto(visualAnomAtual,visualAtual,produto_idproduto) VALUES(#{visualAnomAtual},#{visualAtual},'#{idproduto}');"
	result = client.query(query)
	puts "Log Efetuado : #{ idproduto}"
end



while(true)
	sleep @tempo_entre_logs
	result = $client.query("SELECT idproduto from produto;")
	result.each do |row|
		puts row["idproduto"]
		Thread.new{ monitor(row["idproduto"]) }
	end
end





