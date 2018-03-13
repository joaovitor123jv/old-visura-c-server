#!/bin/ruby

require 'socket'


IP_GOOGLE = "35.192.214.58"
PORTA_INTERFACE = 7420

class Cliente
	@@ip
	@@porta
	@@socket
	@@bufferSize

	@@email
	@@senha

	@@logEnabled
	@@CHAVE_PADRAO
	@@CHAVE_APLICACAO
	@@ehRoot

	def initialize(ip, porta)
		puts "Gerando cliente"
		@ip = ip
		@porta = porta
		@bufferSize = 1024
		@CHAVE_APLICACAO = "APP"
		@CHAVE_PADRAO = "MIGqAgEAAiEArPXCV6pZjBh+OKNVEM/+eIb5KgGZw24jX2mfu2GzFPcCAwEAAQIgCx8KTgTAGUgB5bZq49fb7tzxlVPJya47G9dtFcNrZ7ECEQDgJPU9398P6I0R5MCV+SeVAhEAxYqPkCR6yJ2YuCUZI903WwIRAMmp9swYo8TYYjP7MBq+9JECECeNYMVZqIP2mhd0xh3aMzUCEBGnaCJnmYNciQfqIT1vmws="
		@ehRoot = false
		puts "Cliente inicializado"
	end

	def showLogs(opcao)
		@logEnabled = opcao
	end

	def setBufferSize(size)
		@bufferSize = size
	end

	def send(mensagem)
		if @socket == nil
			return false
		end

		@socket.print(mensagem)

		if @logEnabled
			puts "Mensagem enviada = |#{mensagem}|"
		end

		resposta = @socket.recv(@bufferSize)

		if @logEnabled
			puts "Resposta = |#{resposta}|"
			puts "Codificação da resposta = #{resposta.encoding}"
		end
		return resposta
	end

	def connect2(email, senha)
		@email = email
		@senha = senha

		if @logEnabled
			puts "Conectando"	
		end

		@socket = TCPSocket.open(@ip, @porta)
		if @logEnabled
			puts "Conectado"			
		end

		mensagem = "APP 1 #{@email} #{@senha}"
		@socket.print(mensagem)

		if @logEnabled
			puts "Mensagem enviada = |#{mensagem}|"
		end

		resposta = @socket.recv(1024)

		if @logEnabled
			puts "Resposta = |#{resposta}|"
		end

		if resposta == "OK\0"
			return true
		else
			return false
		end
	end

	def connect
		@email = "interface@servidor"
		@senha = "senatauri"
		@ehRoot = true

		if @logEnabled
			puts "Conectando"	
		end

		@socket = TCPSocket.open(@ip, @porta)
		if @logEnabled
			puts "Conectado"			
		end

		mensagem = "APP 1 #{@email} #{@senha}"
		@socket.print(mensagem)

		if @logEnabled
			puts "Mensagem enviada = |#{mensagem}|"
		end

		resposta = @socket.recv(1024)

		if @logEnabled
			puts "Resposta = |#{resposta}|"
		end

		if resposta == "OK\0"
			return true
		else
			return false
		end
	end

	def encerrar
		@socket.close
	end

	def getChaveAplicacao
		return @CHAVE_APLICACAO
	end

	def getChaveSeguranca
		return @CHAVE_PADRAO
	end

	def show
		puts "Mostrando informações de cliente:"
		puts "IP = #{@ip}"
		puts "porta = #{@porta}"
		puts "socket = #{@socket}"
		puts "bufferSize = #{@bufferSize}"
	end
end