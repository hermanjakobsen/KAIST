import pandas as pd
import os
import math
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats


def continous_return(df):
    cont_return = [] 
    for i in range(len(df.index)):
        try:
            value = math.log(df['Adj Close'][i] / df['Adj Close'][i+1])
            if math.isnan(value):
                cont_return.append(0)
            else:
                cont_return.append(value)
        except:
            cont_return.append(0)
    df['Cont Return'] = cont_return
    df.drop(df.index[-1], inplace=True)
    return df


def condition_dataset(filename, source):
    if source == 'yahoo':
        df = pd.read_csv(filename, usecols=['Date', 'Adj Close'])
        df = df[::-1]
        df.reset_index(drop=True, inplace=True)

    elif source == 'bors':
        df = pd.read_csv(filename, usecols=['OSEBX', 'Siste'])
        df.rename(columns={'OSEBX': 'Date', 'Siste': 'Adj Close'}, inplace=True)
        for i in range(len(df.index)):
            date = df['Date'][i].split('.')
            new_date = '20'+date[2]+'-'+date[1]+'-'+date[0]
            df['Date'].replace({df['Date'][i]: new_date}, inplace=True)
    
    df = continous_return(df)    # Add a column with calculated continous return
    return df


def check_datasets(df1, df2):
    if len(df1.index) != len(df2.index):
        ValueError('The datasets are of different lengt!')
    for i in range(len(df1.index)):
        date1 = df1['Date'][i]
        date2 = df2['Date'][i]
        if date1 != date2:
            print(f'The datasets have inconsistent dates at row {i}.')
            print(f'The dates are {date1} and {date2}.')
            ValueError('Inconsistent dates!')

    
def make_scatter_plot(x, y):
    # Draw scatter points
    plt.scatter(x, y)

    # Draw regression line
    slope, intercept = np.polyfit(x, y, 1)
    plt.plot(x, x*slope + intercept, 'r')

    plt.title('Scatter plot stock vs market')
    plt.ylabel('$r_{stock}$')
    plt.xlabel('$r_{market}$')
    
    plt.grid()
    plt.show()

    return slope, intercept

    
def calculate_beta(r_market, r_stock): # Beta also equals the slope from the linear regression in make_scatter_plot()
    return (np.cov(r_market, r_stock) / np.var(r_market))[1][0]


def main(market_filename, stock_filename):
    stock_df = condition_dataset(stock_filename, 'yahoo')
    market_df = condition_dataset(market_filename, 'bors')
    check_datasets(stock_df, market_df)
    r_stock = np.array(stock_df['Cont Return'].tolist())
    r_market = np.array(market_df['Cont Return'].tolist())

    slope, intercept = make_scatter_plot(r_market, r_stock)
    beta = calculate_beta(r_market, r_stock)

    r = stats.pearsonr(r_market, r_stock)[0]
    print('Beta: ', beta)
    print('Slope: ', slope)
    print('R^2: ', pow(r,2))


main('osebx.csv', 'nordic_semiconductor.csv')