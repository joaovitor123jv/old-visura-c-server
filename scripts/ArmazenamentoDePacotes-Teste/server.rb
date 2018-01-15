require "socket"

# 1 MB de dados
SIZE = 1024 * 1024 

server = TCPServer.open(1212)

class Operacao
	attr_accessor :nomeArquivo, :operacao, :autorizado, :tamanho

	def initialize(operacao, nomeArquivo, autorizado, tamanho)
		@nomeArquivo = nomeArquivo
		@operacao = operacao
		@autorizado = autorizado
		@tamanho = tamanho
	end

	def executarOperacao(cliente)
		if( @operacao == "SEND" ) 
			nomePasta = @nomeArquivo.split('.')
			if( ! Dir.exist?(nomePasta[0]) )
				Dir.mkdir(nomePasta[0])
			end
			File.open(nomePasta[0] +'/'+ @nomeArquivo, 'w') do |file|
				while chunk = cliente.read(SIZE)
					file.write(chunk)
				end
			end

		elsif( @operacao == "RECV" ) 
			if( File.exist?(@nomeArquivo) )
				File.open(@nomeArquivo, 'r') do |file|
					while chunk = file.read(SIZE)
						cliente.write(chunk)
					end
				end
			else
				cliente.write("NOT_FOUND")
			end
		elsif( @operacao == "CHCK" )
			nomePasta = @nomeArquivo.split('.')
			if( ! Dir.exist?(nomePasta[0]))
				cliente.write("ERRO")
			else
				if( !File.exist?(nomePasta[0] + '/' + nomeArquivo) )
					cliente.write("ERRO")
				else
					if( File.size(nomePasta[0] +'/'+nomeArquivo) != @tamanho)
						puts "Tamanho do arquivo (1) = #{File.size(nomePasta[0] + '/'+nomeArquivo)}"
						cliente.write("ERRO")
					else
						puts "Tamanho do arquivo (2) = #{File.size(nomePasta[0] + '/'+nomeArquivo)}"
						cliente.write("OK")
					end
				end
			end
		else
			return cliente.write("ERRO")
		end
	end

	def mostraOperacao
		puts "Operacao: #{@operacao}"
		puts "Nome do Arquivo: #{@nomeArquivo}"
		puts "Tamanho: #{@tamanho}"
		puts "Autorizado: #{@autorizado}"
	end
end

def number_or_nil(string)
  num = string.to_i
  num if num.to_s == string
end


def checaAutorizacao(cliente)
	operacao = Operacao.new("", "", false, 0)
	puts "Checando autorizacao"
	recebido = cliente.gets()
	#recebido = cliente.read(100)
	puts recebido
	params = recebido.split
	if params.length == 3 || params.length == 2 
		if( params[0] == "RECV" && params.length == 2)
			puts "Cliente solicitando um arquivo que existe aqui"
			operacao.operacao = params[0]
			operacao.nomeArquivo = params[1]
			operacao.autorizado = true
		elsif params[0] == "SEND" && params.length == 2
			puts "Cliente solicitando enviar um arquivo pra ca"
			operacao.operacao = params[0]
			operacao.nomeArquivo = params[1]
			operacao.autorizado = true
		elsif params[0] == "CHCK" && params.length == 3
			puts "Cliente solicitando checagem de arquivo"
			operacao.operacao = params[0]
			operacao.nomeArquivo = params[1]
			operacao.tamanho = number_or_nil(params[2])
			operacao.autorizado = true
		else
			puts "Cliente ta doidao"
			operacao.autorizado = false
		end
	end
	#	puts ("Quantidade de splits = #{params.length}")
	#	puts params[0]
	return operacao
	#puts params[1]
end


loop do
	puts "Servidor Aguardando conexao"
	Thread.fork(server.accept) do |client|
		puts "Cliente Conectado"
		operacao = checaAutorizacao(client) 
		if( operacao.autorizado )
			puts "Autorizado a executar a operacao"
		else
			puts "Falha, voce nao foi autorizado tio"
		end
		operacao.mostraOperacao
		#client.puts("CONECTADO")
		operacao.executarOperacao(client)
		#client.puts("Desconectando")
		client.close()
	end
end






