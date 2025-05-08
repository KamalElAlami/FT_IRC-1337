#include "./includes/Server.hpp"
#include <string>

int	 Pars_inputs(std::string Port, std::string Pass)
{
	
	if (Port.empty() || Pass.empty() || Pass.length() > 16 || Pass.length() < 4)
	return (1);
	
	for (size_t i = 0; i < Port.length(); i++)
	if (!std::isdigit(Port[i]))
	return (1);
	
	for (size_t i = 0; i < Pass.length(); i++)
	if (!std::isprint(Pass[i]) || Pass[i] == 32 || Pass[i]  == '\t')
	return (1);
	return (0);
}

int main (int ac, char **av)
{
	int		Port_val;

	if (ac != 3 || Pars_inputs(av[1], av[2]))
	return (std::cerr << "Usage: " << "<port>" << " <pass>" << std::endl, 1);
	Port_val = atoi(av[1]);
	if (Port_val < 6665 || Port_val > 6669)
		return (std::cerr << "Error: Port out of range < from 6665 to 6665 >" << std::endl, 1);
	Server server(-1, atoi(av[1]));
	server.set_Password(av[2]);
	server.Start_Server();	
}