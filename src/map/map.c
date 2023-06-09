/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:47:56 by eayyildi          #+#    #+#             */
/*   Updated: 2023/05/03 11:28:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	check_data_name(char *data_name)
{
	if (ft_strncmp("NO", data_name, 2) == 0)
		return (1);
	if (ft_strncmp("SO", data_name, 2) == 0)
		return (2);
	if (ft_strncmp("WE", data_name, 2) == 0)
		return (3);
	if (ft_strncmp("EA", data_name, 2) == 0)
		return (4);
	if (ft_strncmp("F ", data_name, 2) == 0)
		return (5);
	if (ft_strncmp("C ", data_name, 2) == 0)
		return (6);
	return (-1);
}

int	check_map_helper(t_data *data, int i)
{
	int		data_key_kode;
	char	*data_name;

	data_name = (char *)malloc(sizeof(char) * 3);
	data_name[0] = data->map[i];
	data_name[1] = data->map[i + 1];
	data_name[2] = '\0';
	data_key_kode = check_data_name(data_name);
	if (data_key_kode == -1 || \
	check_double_data(data, data_key_kode) == -1)
	{
		free(data_name);
		return (print_error("Invalid map.\n"));
	}
	free(data_name);
	setup_imgs(data, i + 2, data_key_kode);
	return (0);
}

int	check_map(t_data *data)
{
	int	i;

	i = 0;
	while (data->map[i])
	{
		if (data->map[i] == '\n' || data->map[i] == ' ' \
		|| data->map[i] == '\t') // stringdeki(map dosyası) boşluk, space vb geçilir.
			i++;
		else
		{
			if (check_map_helper(data, i))
				return (1);
			while (data->map[i] != '\n')
				i++;
		}
		if (data->no_bool == 1 && data->so_bool == 1 \
		&& data->we_bool == 1 && data->ea_bool == 1 \
		&& data->f_bool == 1 && data->c_bool == 1) // hepsini dolaşıp bool değerlerini 1 yaptıyda döngüden çıkıyor, imgler alındı
			break ;
	}
	data->map2 = get_data_value(data, i + 1, 0);// data->map2 nin içinde harita var, parse edilmemiş hali  string olarak.
	data->map2_bool = 1;// haritayı aldım SAAAAA
	return (check_map2(data));// harita içinde farklı olmaması gereken bir karakyer varsa hata döndürür.
}

void	init_values(t_data *data)
{
	data->no_bool = 0;
	data->so_bool = 0;
	data->we_bool = 0;
	data->ea_bool = 0;
	data->f_bool = 0;
	data->c_bool = 0;
	data->imgs = (char **)malloc(sizeof(char *) * 7);// map deki ilk 6 satır için yer ayrıldı.
	data->imgs[6] = "\0";
}

int	before_start_game(int fd, t_data *data)
{
	char	*arr;
	char	*temp;
	char	*buff;

	buff = (char *)malloc(sizeof(char) * 2);//map dosyasını bir bir okumak için yer ayırdık.
	buff[1] = '\0';
	arr = (char *)malloc(sizeof(char) * 2);
	arr[1] = '\0';
	while (read(fd, buff, 1))
	{
		temp = ft_strjoin(arr, buff);
		free(arr);
		arr = temp;
	}
	free(buff);
	data->map = arr;// map bir string olarak data->map in içinde. matris değil !!
	init_values(data);// değerleri atadık.
	return (check_map(data));
}
