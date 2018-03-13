#!/usr/bin/ruby

#Para instalar o "httpclient", rode o comando "sudo gem install httpclient" (obviamente o ruby tem que estar instalado antes)
#Cliente HTTP (HTTPClient.new)
require 'httpclient'

require 'i18n'

require './cliente.rb'

client = HTTPClient.new

#Obtem o conteudo do site https://cidades.ibge.gov.br/brasil/sp/monte-aprazivel/panorama

puts "Digite o nome da cidade"
nomeCidadeOriginal = gets.chomp
nomeCidadeOriginal = nomeCidadeOriginal.tr(' ', '_')
cidade = nomeCidadeOriginal.tr('_', '-')
puts "Digite o nome do estado"
nomeEstadoOriginal = gets.chomp
estado = nomeEstadoOriginal.tr('_', '-')


I18n.config.available_locales = :en
cidade = I18n.transliterate(cidade)

cidade.downcase!
estado.downcase!

puts "Cidade = #{cidade}"
puts "Estado = #{estado}"

# conteudo = client.get_content 'https://cidades.ibge.gov.br/brasil/sp/monte-aprazivel/panorama'

url = 'https://cidades.ibge.gov.br/brasil/'+estado+'/'+cidade+'/panorama'

conteudo = client.get_content url

indice = conteudo.index("class=\"lista__valor\" colspan=\"2\">")

if indice == nil
	puts "Merda, não existe esse troço aqui, e agr?"
else
	indice += 51 #tentando achar a posicao certa
	puts conteudo[indice, 50]
end


puts "START !!!"
i = indice
while conteudo[i] != "\n"
	puts "|#{conteudo[i]}|"
	i+=1
end

numeroDeHabitantes = conteudo[indice, i - indice]

puts numeroDeHabitantes



puts "Numero de habitantes, convertido = #{numeroDeHabitantes.tr('.', '')}"


clienteInterface = Cliente.new(IP_GOOGLE, PORTA_INTERFACE)
# clienteInterface = Cliente.new("localhost", PORTA_INTERFACE)

clienteInterface.connect

clienteInterface.showLogs true

mensagem = "APP 2 qC "+nomeCidadeOriginal+" "+nomeEstadoOriginal+" "+numeroDeHabitantes.tr('.', '')

puts "Codificação = #{mensagem.encoding}"

# mensagem.force_encoding(Encoding::ASCII_8BIT)


# default_external = Encoding::ISO_8859_1
default_external = Encoding::ASCII_8BIT


# mensagem.force_encoding(Encoding::ISO_8859_1)

puts "Codificação = #{mensagem.encoding}"

retorno = clienteInterface.send(mensagem)

if retorno == "OK\0"
	puts "Quantidade de habitantes adicionada ao banco de dados com sucesso"
else
	puts "Falha ao adicionar quantidade de habitantes ao banco de dados"
end


puts "fim"
