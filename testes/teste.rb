#!/usr/bin/ruby

vetorDeEntrada = ARGV

puts vetorDeEntrada.length

if vetorDeEntrada.length == 2
	puts "Entrada valida"
	exit 0
else
	puts "Entrada invalida"
	exit 3
end
