#include "./includes/Server.hpp"

int	 Pars_inputs(std::string Port, std::string Pass)
{
	double _port;
	
	if (Pass.length() > 512 || Pass.length() < 4)
		return (std::cerr << "Error: Password length must be between 4 and 512 characters!" << std::endl, 1);
	
	if (Port.empty() || Pass.empty())
		return (std::cerr << "Error: An argument is missing or empty!" << std::endl, 1);
	
	for (size_t i = 0; i < Port.length(); i++)
		if (!std::isdigit(Port[i]))
			return (std::cerr << "Error : Port: Non printable or invalide Ascii " << std::endl, 1);
			
	for (size_t i = 0; i < Pass.length(); i++)
		if (!std::isprint(Pass[i]) || Pass[i] == 32 || Pass[i]  == '\t')
			return (std::cerr << "Error : PASS: Non printable or invalide Ascii " << std::endl, 1);
	
	_port = std::atof(Port.c_str());
	if (_port < 1024 || _port > 65536)
		return (std::cerr << "Error: Port: out of range < from 1024 to 65536 >" << std::endl, 1);
	return (0);
}

void leaks()
{
	system("leaks  ircserv");
}

int main (int ac, char **av)
{
	// atexit(leaks);
	if (ac != 3)
		return (std::cerr << "Error: An argument is missing or empty!" << std::endl, 1);

	try
	{
		signal(SIGINT, Server::SigHandler);
		signal(SIGQUIT, Server::SigHandler);
		signal(SIGPIPE, SIG_IGN);
		if (Pars_inputs(av[1], av[2]))
			return (1);
		Server server(-1, atoi(av[1]));
		server.set_Password(av[2]);
		server.Start_Server();
		// server.ClearAll();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	//std::cout << "pass from here" << std::endl; /*--------------------------------------*/
}