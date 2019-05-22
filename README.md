# NumberPlace of ApacheModule

## How to build this module??

1.  Execute the following command to install Apache HTTPD Server and Apache Develop Tool

	```
	$ sudo apt update
	$ sudo apt install -y apache2 apache2-dev
	```
2. Clone the following repository and initialize submodule (this repository)

	```
	git@github.com:kkato3297/NumberPlace-NumberPlace.git
	```

3. Change the current directory to `mod_numberplace`

4. Execute the following command.

	```
	$ sudo ./build.sh
	$ cd ./CDATA
	$ sudo cp -r ./* /var/www/module/numberplace/
	```

5. Save the following configuration to `/etc/apache2/mods-available/numberplace.conf`.

	``` xml
	<IfModule numberplace_module>
		<Location /numberplace>
			SetHandler numberplace
		</Location>
	</IfModule>
	
	# vim: syntax=apache ts=4 sw=4 sts=4 sr noet
	```

6. Save the following configuration to `/etc/apache2/mods-available/numberplace.load`.

	``` conf
	LoadModule numberplace_module /usr/lib/apache2/modules/mod_numberplace.so
	```

7. Execute the following command.

	```
	$ sudo a2enmod numberplace
	$ sudo systemctl restart apache2
	```

## License

MIT Lisence

## Dependency Library or Software

* Apache HTTPD Server (with Develop Tool)
* jQuery
