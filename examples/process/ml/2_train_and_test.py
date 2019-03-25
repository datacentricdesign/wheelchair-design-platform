import os
import io
from dotenv import load_dotenv
from datetime import datetime
import math

# Import all machine learning computation libraries
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score
from sklearn.metrics import f1_score
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import confusion_matrix
from sklearn.metrics import classification_report
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
from sklearn.feature_selection import VarianceThreshold
import pandas as pd
import numpy

# Import library to generate model
import pickle

from dcd.entities.thing import Thing

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['THING_ID']
THING_TOKEN = os.environ['THING_TOKEN']

# Where to save the model to
MODEL_FILE_NAME = "model.pickle"

# Data collection time frame (in milliseconds)
START_TS = 1553276760000
END_TS = 1553276760000+300000

# Property ID
PROPERTY_DATA = "fsr-7b9c"
PROPERTY_LABEL = "dhaval-bbca"

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can fetch the details of our thing
my_thing.read()
print(my_thing.to_json())

def unix_time_millis(dt):
    epoch = datetime.utcfromtimestamp(0)
    return math.floor((dt - epoch).total_seconds() * 1000.0)

def list_to_df(dataSet):
    dfObj = pd.DataFrame(dataSet)
    return dfObj 

def find_correlation(df, thresh=0.9):
    """
    Given a numeric pd.DataFrame, this will find highly correlated features,
    and return a list of features to remove
    params:
    - df : pd.DataFrame
    - thresh : correlation threshold, will remove one of pairs of features with
               a correlation greater than this value
    """
    
    corrMatrix = df.corr()
    corrMatrix.loc[:,:] =  np.tril(corrMatrix, k=-1)

    already_in = set()
    result = []

    for col in corrMatrix:
        perfect_corr = corrMatrix[col][corrMatrix[col] > thresh].index.tolist()
        if perfect_corr and col not in already_in:
            already_in.update(set(perfect_corr))
            perfect_corr.append(col)
            result.append(perfect_corr)


    select_nested = [f[1:] for f in result]
    select_flat = [i for j in select_nested for i in j]
    print(select_flat)
    return select_flat

def variance(train, validate, test):
    '''
    test the method of VarianceThreshold
    :return:  None
    '''

    selector=VarianceThreshold(1)
    selector.fit(train)
    train1 = selector.transform(train)

    print("Variances is %s"%selector.variances_)
    print("The support is %s"%selector.get_support(True))
    print("After transform is %s"%train1)
    print("After reverse transform is %s" %selector.inverse_transform(selector.transform(train)))

    validate = selector.transform(validate)
    test = selector.transform(test)

    return train1, validate, test

def pca(train, validate, test):
    # Scaling for optimization of algorithm.
    scaler = StandardScaler()
    
    # Fit on training set only.
    scaler.fit(train)
    
    # Apply transform to both the training set and validation set.
    train = scaler.transform(train)
    validate = scaler.transform(validate) 

    # Choose minimum number of Principal Components such that 95% variance is retained and create PCA model
    modelpca = PCA(.95)

    # Apply PCA to training and validation sets
    train = modelpca.transform(train)
    validate = modelpca.transform(validate)
    test = modelpca.transform(test)

    return train

# def train_knn_classifier(train, label):
#     neigh = KNeighborsClassifier(n_neighbors=1)
#     neigh.fit(train_data, train_label)

def generate_confusion_matrix(labels):
    # Generate parameters to test our model
    
    matrix = confusion_matrix(labels, predicted)
    print(matrix)
    print(precision_score(labels, predicted, average="macro"))
    print(recall_score(labels, predicted, average="macro"))
    print(f1_score(labels, predicted, average="weighted"))
    print(f1_score(labels, predicted, average=None))
   


# If you just registered your Thing on the DCD Hub,
# it has only an id, a name and a type.
# print(my_thing.to_json())

fsr = my_thing.find_property_by_name(PROPERTY_DATA)
fsr.read(START_TS, END_TS)

sitting = my_thing.find_property_by_name(PROPERTY_LABEL)
sitting.read(START_TS, END_TS)

classes = []
for clazz in sitting.classes:
    classes.append(clazz['name'])

data = fsr.values
label = sitting.values
# print(label)

# Split the data into training data (60%), cross validation data(20%) and test data (20%)
train_data = []
train_label = []
cv_data = []
cv_label = []
test_data = []
test_label = []
leftover_data = []
leftover_label = []

for index in range(len(data)):
    # remove time
    data[index].pop(0)
    label[index].pop(0)
    if index%5 == 0:
        # 20% to test data
        test_data.append(data[index])
        test_label.append(label[index])
    else:
        # 80% leftover data
        leftover_data.append(data[index])
        leftover_label.append(label[index])

for index in range(len(leftover_data)):
    if index%4 == 0:
        # 20% to cross validate
        cv_data.append(leftover_data[index])
        cv_label.append(leftover_label[index])
    else:
        # 60% to train
        train_data.append(leftover_data[index])
        train_label.append(leftover_label[index])

# print("nb total data: " + str(len(data)))
# print("nb total labels: " + str(len(label)))

# print("nb train data: " + str(len(train_data)))
# print("nb train labels: " + str(len(train_label)))

# print("nb cv data: " + str(len(cv_data)))
# print("nb cv labels: " + str(len(cv_label)))

# print("nb test data: " + str(len(test_data)))
# print("nb test labels: " + str(len(test_label)))

# Check if data is being pulled from the hub

# print("TRAINING DATA")
# print(train_data)

# print("VALIDATION DATA")
# print(cv_data)

# print("TESTING DATA")
# print(test_data)

# Apply dimensionality reduction. Choose only one of the below lines at one time. 
# train_data, cv_data, test_data = pca(train_data, cv_data, test_data)
# train_data= variance(train_data, cv_data, test_data)

# print(train_data)

# print(test_label)
# print(label)
# print(leftover_label)

print(len(data))
print(len(label))

# Train a k-Nearest Neighbour (kNN) algorithm
neigh = KNeighborsClassifier(n_neighbors=1)
neigh.fit(train_data, train_label)

# Use the test data to evaluate the algorithm
predicted = neigh.predict(cv_data)
cvLabel = numpy.array(cv_label)
result = accuracy_score(cvLabel, predicted)
print("cv accuracy: {}".format(result))

if result > 0.8:
    print("Validation passed. Displaying testing performance")
    predicted = neigh.predict(test_data)
    testLabel = numpy.array(test_label)
    result = accuracy_score(testLabel, predicted)
    print("test accuracy: {}".format(result))
   
    # Report evaluation Confusion matrix
    generate_confusion_matrix(testLabel)
    print(classification_report(testLabel, predicted, target_names=classes))
else:
    print("Validation failed. Displaying Validation performance")
    
    # Report evaluation Confusion matrix
    generate_confusion_matrix(cvLabel)

# Save the model in a file
with io.open(MODEL_FILE_NAME, "wb") as file:
    pickle.dump(neigh, file, protocol=2)