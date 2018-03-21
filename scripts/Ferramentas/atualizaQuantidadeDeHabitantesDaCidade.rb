#!/usr/bin/ruby




#Esse script atualiza a quantidade de habitantes do numero da cidade na interface, gerando um comando e executando como usuario ROOT



#Para instalar o "httpclient", rode o comando "sudo gem install httpclient" (obviamente o ruby tem que estar instalado antes)
#Cliente HTTP (HTTPClient.new)
require 'httpclient'

require 'i18n'

require './cliente.rb'

client = HTTPClient.new

#Obtem o conteudo do site https://cidades.ibge.gov.br/brasil/sp/monte-aprazivel/panorama

nomeCidadeOriginal = ""
nomeEstadoOriginal = ""

mostrarLogs = true

if ARGV.length == 0
	puts "Digite o nome da cidade"
	nomeCidadeOriginal = gets.chomp
	
	puts "Digite o nome do estado"
	nomeEstadoOriginal = gets.chomp

else
	nomeCidadeOriginal = String(ARGV[0])
	nomeEstadoOriginal = String(ARGV[1])
	mostrarLogs = false
end

nomeCidadeOriginal = nomeCidadeOriginal.tr(' ', '_')
cidade = nomeCidadeOriginal.tr('_', '-')
estado = nomeEstadoOriginal.tr('_', '-')


I18n.config.available_locales = :en
cidade = I18n.transliterate(cidade)

cidade.downcase!
estado.downcase!

if mostrarLogs
	puts "Cidade = #{cidade}"
	puts "Estado = #{estado}"
end


# conteudo = client.get_content 'https://cidades.ibge.gov.br/brasil/sp/monte-aprazivel/panorama'

url = 'https://cidades.ibge.gov.br/brasil/'+estado+'/'+cidade+'/panorama'

conteudo = client.get_content url

indice = conteudo.index("class=\"lista__valor\" colspan=\"2\">")

if indice == nil
	if mostrarLogs
		puts "Merda, não existe esse troço aqui, e agr?"
	end
	exit ERRO
else
	indice += 51 #tentando achar a posicao certa
	if mostrarLogs
		puts conteudo[indice, 50]
	end
end

if mostrarLogs
	puts "START !!!"
end
i = indice
while conteudo[i] != "\n"
	if mostrarLogs
		puts "|#{conteudo[i]}|"
	end
	i+=1
end

numeroDeHabitantes = conteudo[indice, i - indice]

if mostrarLogs
	puts numeroDeHabitantes
	puts "Numero de habitantes, convertido = #{numeroDeHabitantes.tr('.', '')}"
end

clienteInterface = Cliente.new(IP_GOOGLE, PORTA_INTERFACE)
# clienteInterface = Cliente.new("localhost", PORTA_INTERFACE)

clienteInterface.connect

if mostrarLogs
	clienteInterface.showLogs true
else
	clienteInterface.showLogs false
end


mensagem = "APP 2 qC "+nomeCidadeOriginal+" "+nomeEstadoOriginal+" "+numeroDeHabitantes.tr('.', '')

if mostrarLogs
	puts "Codificação = #{mensagem.encoding}"
end

# mensagem.force_encoding(Encoding::ASCII_8BIT)


# default_external = Encoding::ISO_8859_1
default_external = Encoding::ASCII_8BIT


# mensagem.force_encoding(Encoding::ISO_8859_1)

if mostrarLogs
	puts "Codificação = #{mensagem.encoding}"
end

retorno = clienteInterface.send(mensagem)


if retorno == "OK\0"
	puts " LOG : Quantidade de habitantes adicionada ao banco de dados com sucesso (script externo) scripts/Ferramentas/atualizaQuantidadeDeHabitantesDaCidade.rb"
	exit 0
else
	puts " LOG : Falha ao adicionar quantidade de habitantes ao banco de dados (script externo) scripts/Ferramentas/atualizaQuantidadeDeHabitantesDaCidade.rb"
	exit -1
end
