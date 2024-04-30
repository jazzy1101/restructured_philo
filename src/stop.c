/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabae <dabae@student.42perpignan.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:26:18 by dabae             #+#    #+#             */
/*   Updated: 2024/04/30 15:20:51 by dabae            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

int	monitoring_num_eat(t_data *data, int i)
{
	mutex_handler(data, &data->full_lock, LOCK);
	check_full(&data->philo[i]);
	if (data->num_full == data->num_must_eat)
	{
		mutex_handler(data, &data->full_lock, UNLOCK);
		return (1);
	}
	mutex_handler(data, &data->full_lock, UNLOCK);
	return	(0);
}

void	check_full(t_philo *philo)
{
	if (philo->is_full == false)
	{
		//mutex_handler(philo->data, &philo->data->full_lock, LOCK);
		if (philo->num_eat == philo->data->num_must_eat)
		{
			philo->is_full = true;
			philo->data->num_full++;
			//mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
		}
		if (philo->data->num_full == philo->data->num_philo)
		{
			philo->data->stop = true;
			mutex_handler(philo->data, &philo->data->print_lock, LOCK);
			printf("All philosophers have eaten as many times as %d.\n", philo->data->num_philo);
			mutex_handler(philo->data, &philo->data->print_lock, UNLOCK);
		}
		//mutex_handler(philo->data, &philo->data->full_lock, UNLOCK);
	}
	// mutex_handler(philo->data, &philo->num_eat_lock, LOCK);
	// if (philo->num_eat == philo->data->num_must_eat)
	// {
	// 	philo->is_full = true;
	// 	philo->data->num_full++;
	// 	mutex_handler(philo->data, &philo->num_eat_lock, UNLOCK);
	// 	return (1);
	// }
	// mutex_handler(philo->data, &philo->num_eat_lock, UNLOCK);
}

void	check_death(t_philo *philo)
{
	if (philo->data->num_philo == 1)
	{
		print(philo, "died");
		philo->data->stop = true;
		return ;
	}
	mutex_handler(philo->data, &philo->last_meal_lock, LOCK);
	mutex_handler(philo->data, &philo->eating_lock, LOCK);
	if (philo->is_eating == 0 && philo->last_meal && get_time() >= philo->last_meal + philo->data->time_to_die)
	{
		mutex_handler(philo->data, &philo->eating_lock, UNLOCK);
		mutex_handler(philo->data, &philo->last_meal_lock, UNLOCK);
		print(philo, "died");
		philo->data->stop = true;
	}
	else
	{
		mutex_handler(philo->data, &philo->eating_lock, UNLOCK);	
		mutex_handler(philo->data, &philo->last_meal_lock, UNLOCK);
	}
}

void	check_to_stop(t_data *data)
{
	int	i;
	int	full_count;

	full_count = 0;
	while (1)
	{
		i = -1;
		while (++i < data->num_philo)
		{
			if (data->num_must_eat > 0)
			{
				if (monitoring_num_eat(data, i))
					return ;
			}
			mutex_handler(data, &data->stop_lock, LOCK);
			check_death(&data->philo[i]);
			if (data->stop)
			{
				mutex_handler(data, &data->stop_lock, UNLOCK);
				return ;
			}
			mutex_handler(data, &data->stop_lock, UNLOCK);
		}
	}
}
