require 'socket'

HOST_PADRAO_CONEXAO_INTERNA_COM_SERVIDOR = "localhost"
PORTA_PADRAO_DE_CONEXAO_INTERNA_COM_SERVIDOR = 13598

class ConexaoInternaComServer
	@@id
	@@host
	@@porta
	@@socket
	def initialize (id)
		@id = id
		@host = HOST_PADRAO_CONEXAO_INTERNA_COM_SERVIDOR
		@porta = PORTA_PADRAO_DE_CONEXAO_INTERNA_COM_SERVIDOR
	end

	def conectar
		@socket = TCPSocket.open(@host, @porta)
	end

	def enviaResposta (resposta)
		conectar()
		if @socket == nil
			puts "ERRO: RUBY DEU RUIM, falha ao criar socket com wrapper"
			exit -8
		end
		mensagem = "#{@id} #{resposta}"
		tentando = true
		contador = 1

		while tentando
			@socket.print(mensagem)

			resposta = @socket.recv(1024)
			if resposta == "OK\0"
				@socket.close
				tentando = false
				exit 0
			else
				tentando = true
				contador = contador + 1
			end

			if contador > 100
				puts "ERRO: RUBY NÃO CONSEGUIU SE CONECTAR COM WRAPPER"
				@socket.close
				tentando = false
				exit -7
			end
		end
	end

end