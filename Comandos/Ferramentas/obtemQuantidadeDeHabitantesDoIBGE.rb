#!/usr/bin/ruby
# ESSE código DEVE ser chamado utilizando essa forma de comunicação
# 				"./nomeAplicacao ARGV[0] ARGV[1] ID"
# Esse código checa a quantidade de habitantes que uma cidade possui, criando uma conexão HTTPS com o site do IBGE
# após obter a quantidade de habitantes da cidade, ele abre um socket na porta padrão para conexões entre Wrapper e as ferramentas
# e então retorna a quantidade através de um socket para "localhost:porta" no seguinte formato:
#				"ID resposta"



require 'httpclient'

require 'i18n'
# require './Comandos/Ferramentas/ConexaoInternaComServer.rb'
require './Comandos/Ferramentas/ConexaoInternaComServer.rb'


if ARGV.length != 3
	exit -1
end


client = HTTPClient.new

nomeCidadeOriginal = String(ARGV[0])
nomeEstadoOriginal = String(ARGV[1])
id = String(ARGV[2])

cidade = nomeCidadeOriginal.tr('_', '-')
estado = nomeEstadoOriginal.tr('_', '-')

I18n.config.available_locales = :en
cidade = I18n.transliterate(cidade)

cidade.downcase!
estado.downcase!

url = 'https://cidades.ibge.gov.br/brasil/'+estado+'/'+cidade+'/panorama'

conteudo = client.get_content url

indice = conteudo.index("class=\"lista__valor\" colspan=\"2\">")

conexao = ConexaoInternaComServer.new(id)

if indice == nil
	conexao.enviaResposta("ERRO")
	exit 0
end

indice += 51 #tentando achar a posicao certa

i = indice

while conteudo[i] != "\n"
	i+=1
end

numeroDeHabitantes = conteudo[indice, i - indice]

conexao.enviaResposta(numeroDeHabitantes.tr('.', ''))