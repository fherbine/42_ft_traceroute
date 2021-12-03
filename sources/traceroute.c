#include "../includes/ft_traceroute.h"


static void set_probes_timeout(t_traceroute *tracert, double timeout_value)
{
	struct timeval timeout;      
    timeout.tv_sec = (int) timeout_value;
    timeout.tv_usec = (int)(timeout_value * 100000);

	if (setsockopt(tracert->rcv_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0){
		perror("setsockopt"); message_exit("cannot set probes timeout.", EXIT_FAILURE, 0); }
}

static void create_sockets(t_traceroute *tracert)
{
	uint8_t sock_type, sock_proto;

	sock_type = (tracert->options & TRACERT_OPT_ICMP) ? SOCK_RAW : SOCK_DGRAM;
	sock_proto = (tracert->options & TRACERT_OPT_ICMP) ? IPPROTO_ICMP : 0;

	if ((tracert->snd_socket = socket(AF_INET, sock_type, sock_proto)) == -1)
	{
		free_tracert(tracert);
		message_exit("Cannot create socket. Are you root ?", EXIT_FAILURE, 0);
	}

	if ((tracert->rcv_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
	{
		free_tracert(tracert);
		message_exit("Cannot create rcv socket. Are you root ?", EXIT_FAILURE, 0);
	}
}

void	ft_traceroute(t_traceroute *tracert)
{
	uint8_t buffer_size_udp = PACKETSIZE - (sizeof(struct iphdr) + UDP_HEADER_SIZE);
	uint8_t buffer_size_icmp = PACKETSIZE - sizeof(struct iphdr);
	char snd_buffer[PACKETSIZE];
	char rcv_buffer[1024];
	uint8_t hop, last_icmp_type = ICMP_TIME_EXCEEDED;
	struct in_addr last_ip;

	t_sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
       
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(33434);
    servaddr.sin_addr.s_addr = tracert->ipv4.sin_addr.s_addr;

	// bzeros
	ft_bzero(snd_buffer, PACKETSIZE);
	ft_bzero(&last_ip, sizeof(last_ip));

	// prepare request ICMP (-I)
	struct icmphdr echo_icmp;
	ft_bzero(&echo_icmp, sizeof(echo_icmp));
	echo_icmp.type = ICMP_ECHO;
	echo_icmp.un.echo.id = (uint16_t)getpid();
	echo_icmp.checksum = compile_checksum(&echo_icmp, sizeof(echo_icmp));

	if (tracert->options & TRACERT_OPT_ICMP)
		ft_memcpy(snd_buffer, &echo_icmp, sizeof(echo_icmp));

	// Création socket
	create_sockets(tracert);

	// probes timeouts
	set_probes_timeout(tracert, PROBES_TIMEOUT);

	printf("traceroute to %s (%s), %d hops max, %d byte packets\n", \
		tracert->name_or_service, inet_ntoa(tracert->ipv4.sin_addr), \
		MAXHOPS, PACKETSIZE);

	for (hop = 1; hop <= MAXHOPS; hop++)
	{
		if (last_icmp_type != ICMP_TIME_EXCEEDED)
			break ;

		if (setsockopt(tracert->snd_socket, IPPROTO_IP, IP_TTL, &hop, sizeof(hop)) < 0)
			message_exit("cannot set unicast time-to-live: Invalid argument", EXIT_FAILURE, 0);
		
		ft_bzero(&last_ip, sizeof(last_ip));

		printf("%2d  ", hop);

		for (uint8_t probe = 0; probe < 3; probe++)
		{
			int8_t bsent = sendto(tracert->snd_socket, \
				snd_buffer, \
				(tracert->options & TRACERT_OPT_ICMP) ? buffer_size_icmp : buffer_size_udp, \
				0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
			if (bsent < 0)
			{
				if (errno == 101) {
					message_exit("connect: Network is unreachable", EXIT_FAILURE, 0);
				}
				message_exit("Cannot send traceroute probe.", EXIT_FAILURE, 0);
			}
			t_time sent_ts = getnow();

			ssize_t breceived = recvfrom(tracert->rcv_socket, (char *)rcv_buffer, 1023, MSG_WAITALL,
			 NULL, NULL);
			if (breceived < 0)
			{
				if (errno == EAGAIN)
					{
						printf("* ");
						continue ;
					}
				else
					message_exit("Cannot receive message.", EXIT_FAILURE, 0);
			}

			t_time received_ts = getnow();
			struct ip *ip = (struct ip *)rcv_buffer;
			struct icmphdr *icmp = (void *)rcv_buffer + sizeof(struct iphdr);
			last_icmp_type = icmp->type;

			if (last_ip.s_addr != ip->ip_src.s_addr) {
				t_sockaddr_in srcip;
				ft_bzero(&srcip, sizeof(srcip));
				srcip.sin_family = AF_INET;
				srcip.sin_addr = ip->ip_src;
				char *host = "";
				reversedns4(&srcip, &host);
				printf("%s (%s) ", host, inet_ntoa(ip->ip_src));
				free(host);
				last_ip = ip->ip_src;
			}
			printf(" %.3f ms ", (received_ts - sent_ts));
			servaddr.sin_port = htons(ntohs(servaddr.sin_port) + 1);
		}
		printf("\n");
	}

	close(tracert->snd_socket);
	close(tracert->rcv_socket);
}