from pulp import *
import sys
from typing import List, Dict


class Country:
    def __init__(self, export: int, distribution: int):
        self._max_export = export
        self._min_distribution = distribution
        self._factories = {}

    def max_export(self) -> int:
        return self._max_export

    def min_distribution(self) -> int:
        return self._min_distribution

    def factories(self) -> Dict[int, "Factory"]:
        return self._factories

    def __str__(self):
        return (
            "max_export: "
            + str(self.max_export())
            + ", min_distribution: "
            + str(self.min_distribution())
        )


class Factory:
    def __init__(self, stock: int):
        self._stock = stock
        self._inner_orders = []
        self._outer_orders = []

    def stock(self) -> int:
        return self._stock

    def inner_orders(self) -> List[LpVariable]:
        return self._inner_orders

    def outer_orders(self) -> List[LpVariable]:
        return self._outer_orders

    def __str__(self):
        return "Stock: " + str(self.stock())


input = sys.stdin.read()
input = input.splitlines()

factories, countries, children = map(int, input[0].split(" "))

# Create a list of factories and countries
factories_list: List[Factory] = [None] * factories
countries_list: List[Country] = [None] * countries

# Fill Countries
for i in range(factories + 1, factories + countries + 1):
    line = input[i].split(" ")  # id, export, distribution
    countries_list[int(line[0]) - 1] = Country(int(line[1]), int(line[2]))

# Fill Factories
for i in range(1, factories + 1):
    line = input[i].split(" ")  # id, country, stock

    # Set the stock values for each factory.
    factory = Factory(int(line[2]))
    factories_list[int(line[0]) - 1] = factory

    # Add factory to country
    countries_list[int(line[1]) - 1].factories()[int(line[0]) - 1] = factory


# Create the LP problem
prob = LpProblem("Gifts_received", LpMaximize)
variables = []

# Fill Gift Wishes
for i in range(factories + countries + 1, factories + countries + children + 1):
    line = input[i].split(" ")  # id, country, factories..
    child_id = line[0]
    country_id = line[1]
    restriction = []

    for j in range(2, len(line)):
        factory_id = line[j]
        # Create a variable id, factory, country for LP problem
        variable = LpVariable(
            f"x{child_id}_c{country_id}_f{factory_id}", 0, 1, cat="Binary"
        )
        restriction.append(variable)

        # Add the order to the factory or country
        factory = (
            countries_list[int(country_id) - 1].factories().get(int(factory_id) - 1)
        )
        if factory:
            factory.inner_orders().append(variable)
        else:
            factories_list[int(factory_id) - 1].outer_orders().append(variable)

    prob += (lpSum(restriction) <= 1, "Wishes of Child " + child_id)
    variables.extend(restriction)

# Add the objective function
prob += lpSum(variables), "Objective Function"

# Input no longer needed
del input, factories, countries, children, variables

# Add the restrictions for the Factory stocks
for factory_index, factory in enumerate(factories_list):
    # If there are any orders, add the "max stock" constraint
    if (len(factory.outer_orders()) + len(factory.inner_orders())) > 0:
        prob += (
            lpSum(factory.outer_orders()) + lpSum(factory.inner_orders())
            <= factory.stock(),
            "Max Stock of Factory " + str(factory_index + 1),
        )

# Add the restrictions for the Country export and distribution
for country_index, country in enumerate(countries_list):
    # Collect outer order variables
    outer_orders_list = [
        lpSum(factory.outer_orders())
        for factory in country.factories().values()
        if len(factory.outer_orders()) > 0
    ]

    # Collect inner order variables
    inner_orders_list = [
        lpSum(factory.inner_orders())
        for factory in country.factories().values()
        if len(factory.inner_orders()) > 0
    ]

    # If there are any outer orders, add the "max export" constraint
    if outer_orders_list:
        prob += (
            lpSum(outer_orders_list) <= country.max_export(),
            "Max Export of Country {}".format(country_index + 1),
        )

    # If there are any inner orders, add the "min distribution" constraint
    if inner_orders_list:
        prob += (
            lpSum(inner_orders_list) >= country.min_distribution(),
            "Min Distribution of Country {}".format(country_index + 1),
        )


# # Print the Countries and Factories
# for country_index, country in enumerate(countries_list):
#     print("================== Country " + str(country_index + 1) + "==================")
#     print(country)
#     for factory_index, factory in country.factories().items():
#         print(
#             "------------------ Factory "
#             + str(factory_index + 1)
#             + "------------------"
#         )
#         print(factory)
#         print("Inner Orders: ", [v.name for v in factory.inner_orders()])
#         print("Outer Orders: ", [v.name for v in factory.outer_orders()])

# Remove the variables that are not used
del factories_list, countries_list

# Solve the problem
prob.solve(GLPK(msg=0))
if prob.status == pulp.const.LpSolutionOptimal:
    print(value(prob.objective))
else:
    print("-1")
