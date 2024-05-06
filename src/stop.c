/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:26:18 by dabae             #+#    #+#             */
/*   Updated: 2024/05/06 18:21:18 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	check_full(t_philo *philo)
{
	if (philo->is_full == false)
	{
		mutex_handler(philo->data, &philo->num_eat_lock, LOCK);
		if (philo->num_eat == philo->data->num_must_eat)
		{
			philo->is_full = true;
			mutex_handler(philo->data, &philo->data->full_lock, LOCK);
			philo->data->num_full++;
			mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
		}
		mutex_handler(philo->data, &philo->num_eat_lock, UNLOCK);
		mutex_handler(philo->data, &philo->data->full_lock, LOCK);
		if (philo->data->num_full == philo->data->num_philo)
		{
			mutex_handler(philo->data, &philo->data->print_lock, LOCK);
			printf("All philosophers have eaten at least %d times.\n", \
				philo->data->num_must_eat);
			mutex_handler(philo->data, &philo->data->print_lock, UNLOCK);
			mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
			return (1);
		}
		mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
	}
	return (0);
}

int	check_death(t_philo *philo)
{
	if (philo->data->num_philo == 1)
	{
		print(philo, "died");
		set_stop(philo->data);
		return (1);
	}
	mutex_handler(philo->data, &philo->eating_lock, LOCK);
	mutex_handler(philo->data, &philo->start_time_lock, LOCK);
	if (philo->is_eating == 0 && philo->start_time && get_time() >= \
		philo->start_time + philo->data->time_to_die)
	{
		print(philo, "died");
		set_stop(philo->data);
		mutex_handler(philo->data, &philo->eating_lock, UNLOCK);
		mutex_handler(philo->data, &philo->start_time_lock, UNLOCK);
		return (1);
	}
	else
	{
		mutex_handler(philo->data, &philo->eating_lock, UNLOCK);
		mutex_handler(philo->data, &philo->start_time_lock, UNLOCK);
	}
	return (0);
}

static int	all_full(t_data *data)
{
	mutex_handler(data, &data->monitor_lock, LOCK);
	mutex_handler(data, &data->full_lock, LOCK);
	if (data->num_full == data->num_philo)
	{
		mutex_handler(data, &data->monitor_lock, UNLOCK);
		mutex_handler(data, &data->full_lock, UNLOCK);
		return (1);
	}
	mutex_handler(data, &data->monitor_lock, UNLOCK);
	mutex_handler(data, &data->full_lock, UNLOCK);
	return (0);
}

void	check_to_stop(t_data *data)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < data->num_philo)
		{
			if (data->num_must_eat > 0)
			{
				if (all_full(data))
					return ;
			}
			check_death(&data->philo[i]);
			mutex_handler(data, &data->exit_lock, LOCK);
			mutex_handler(data, &data->stop_lock, LOCK);
			if (data->stop)
			{
				mutex_handler(data, &data->exit_lock, UNLOCK);
				mutex_handler(data, &data->stop_lock, UNLOCK);
				return ;
			}
			mutex_handler(data, &data->exit_lock, UNLOCK);
			mutex_handler(data, &data->stop_lock, UNLOCK);
		}
	}
}
