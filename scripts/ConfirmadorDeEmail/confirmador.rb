#Fazer esse negócio enviar um email de confirmação
#
#Envia um email pra algum lugar, usando SMTP
#
#
#require 'net/smtp'
#
#message = <<MESSAGE_END
#From: Eu mesmo Person <jv.joaovitoras@gmail.com>
#To: A Test User <jv.joaovitoras@outlook.com>
#Subject: Teste e-mail teste
#
#This is a test e-mail message.
#MESSAGE_END
#
#Net::SMTP.start('localhost') do |smtp|
#  smtp.send_message message, 'jv.joaovitoras@gmail.com', 
#                             'jv.joaovitoras@outlook.com'
#end
#
#
#
#
#Pro metodo abaixo funcionar, execute 'gem install mail'

require 'mail'

mail = Mail.new do
	from 'jv.joaovitoras@gmail.com'
	to	'jv.joaovitoras@outlook.com'
	subject	'Um email de teste'
	body File.read('body.txt')
end

mail.to_s

mail.delivery_method :sendmail


mail.deliver!


puts "Email enviado ou não"
