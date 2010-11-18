



int networkinit() {

    struct vpxsocket vpx_socket, vpx_socket2;

    union vpx_sockaddr_x address, address2;

    TCRV rc;

    int i;

    int bytes_read;

    vpx_net_init();

    // data send socket
    FAIL_ON_NONZERO(vpx_net_open(&vpx_socket, vpx_IPv4, vpx_UDP))
    FAIL_ON_NONZERO(vpx_net_get_addr_info(ip, send_port, vpx_IPv4, vpx_UDP, &address))
    // feedback socket
    FAIL_ON_NONZERO(vpx_net_open(&vpx_socket2, vpx_IPv4, vpx_UDP))
    vpx_net_set_read_timeout(&vpx_socket2, 0);

    rc = vpx_net_bind(&vpx_socket2, 0 , recv_port);

    vpx_net_set_send_timeout(&vpx_socket, vpx_NET_NO_TIMEOUT);

    // make sure 2 way discussion taking place before getting started

    int bytes_sent;

#ifndef ONEWAY

    while (!_kbhit())
    {
        char init_packet[PACKET_SIZE] = "initiate call";
        rc = vpx_net_sendto(&vpx_socket, (tc8 *) &init_packet, PACKET_SIZE , &bytes_sent, address);
        Sleep(200);

        rc = vpx_net_recvfrom(&vpx_socket2, one_packet, sizeof(one_packet), &bytes_read, &address2);

        if (rc != TC_OK && rc != TC_WOULDBLOCK)
            vpxlog_dbg(LOG_PACKET, "error\n");

        if (bytes_read == -1)
            bytes_read = 0;

        if (bytes_read)
        {
            if (strncmp(one_packet, "configuration ", 14) == 0)
            {
                sscanf(one_packet + 14, "%d %d %d %d %d %d", &display_width, &display_height, &capture_frame_rate, &video_bitrate, &fec_numerator, &fec_denominator);
                printf("Dimensions: %dx%-d %dfps %dkbps %d/%dFEC\n", display_width, display_height, capture_frame_rate, video_bitrate, fec_numerator, fec_denominator);
                break;
            }
        }
    }

    char init_packet[PACKET_SIZE] = "confirmed";
    rc = vpx_net_sendto(&vpx_socket, (tc8 *) &init_packet, PACKET_SIZE , &bytes_sent, address);
    Sleep(200);
    rc = vpx_net_sendto(&vpx_socket, (tc8 *) &init_packet, PACKET_SIZE , &bytes_sent, address);
    Sleep(200);
    rc = vpx_net_sendto(&vpx_socket, (tc8 *) &init_packet, PACKET_SIZE , &bytes_sent, address);
#endif

    return 0;
}


int packetandsend() {

}
