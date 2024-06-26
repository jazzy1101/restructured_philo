/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:26:21 by dabae             #+#    #+#             */
/*   Updated: 2024/05/07 18:15:09 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	cleanup(t_data *data)
{
	if (data->tids)
		free(data->tids);
	if (data->forks)
		free(data->forks);
	if (data->philo)
		free(data->philo);
	free(data);
}

void	ft_exit(t_data *data, int err, char *msg)
{
	int	i;

	i = -1;
	if (!data)
		return ;
	while (++i < data->num_philo)
	{
		mutex_handler(data, &data->philo[i].num_eat_lock, DESTROY);
		mutex_handler(data, &data->philo[i].eating_lock, DESTROY);
		mutex_handler(data, &data->philo[i].start_time_lock, DESTROY);
	}
	mutex_handler(data, &data->print_lock, DESTROY);
	mutex_handler(data, &data->stop_lock, DESTROY);
	mutex_handler(data, &data->full_lock, DESTROY);
	mutex_handler(data, &data->monitor_lock, DESTROY);
	cleanup(data);
	if (err)
	{
		printf("%s\n", msg);
		exit(1);
	}
	else
		exit(0);
}

void	join_threads(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philo)
	{
		if (pthread_join(data->tids[i], NULL) == -1)
			ft_exit(data, 1, "Error: pthread_join failed");
	}
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac == 5 || ac == 6)
	{
		if (is_digit(av + 1) && is_positive(av + 1))
		{
			data = malloc(sizeof(t_data));
			if (!data)
				return (1);
			init_data(data, av + 1);
			init_philo(data);
			if (data->num_philo == 1)
				check_death(&data->philo[0]);
			else
			{
				life_cycle(data);
				check_to_stop(data);
				join_threads(data);
			}
			ft_exit(data, 0, NULL);
		}
	}
	printf("Invalid arguments");
	return (1);
}
