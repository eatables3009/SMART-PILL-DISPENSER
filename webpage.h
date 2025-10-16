#ifndef WEBPAGE_H_
#define WEBPAGE_H_

// This entire string holds your web application code
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Pill Dispenser</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;600;700;800&display=swap');
        body {
            font-family: 'Inter', sans-serif;
            background-color: #e3f2fd;
        }
        .btn-gradient-sky {
            background-image: linear-gradient(to right, #0ea5e9, #38bdf8);
        }
        .btn-gradient-sky:hover {
            background-image: linear-gradient(to right, #38bdf8, #0ea5e9);
        }
        .btn-gradient-teal {
            background-image: linear-gradient(to right, #14b8a6, #2dd4bf);
        }
        .btn-gradient-teal:hover {
            background-image: linear-gradient(to right, #2dd4bf, #14b8a6);
        }
        .alert-pulse {
            animation: pulse-ring 1s infinite;
        }
        @keyframes pulse-ring {
            0%, 100% { box-shadow: 0 0 0 0 rgba(251, 191, 36, 0.8); }
            50% { box-shadow: 0 0 0 10px rgba(251, 191, 36, 0); }
        }
        .day-box {
            @apply flex justify-center items-center p-2 rounded-lg cursor-pointer transition duration-150 border border-transparent;
        }
        .day-checkbox:checked + .day-label .day-box {
            @apply bg-teal-500 text-white border-teal-600 shadow-md;
        }
        .day-checkbox + .day-label .day-box {
            @apply bg-sky-100/50 text-gray-700 hover:bg-sky-200/70 border-gray-300;
        }
        .day-checkbox {
            @apply absolute w-0 h-0 opacity-0;
        }
    </style>
</head>
<body class="text-gray-800">

    <div id="landing-page" class="min-h-screen p-4 sm:p-8 flex flex-col items-center">
        <header class="w-full max-w-5xl text-center py-12">
            <h1 class="text-5xl md:text-6xl font-extrabold text-gray-900 tracking-tight mb-4">
                Smart Pill Dispenser
            </h1>
            <p class="text-xl text-gray-600 max-w-3xl mx-auto">
                Your intelligent partner in health management. Never miss a dose again with our automated and easy-to-use scheduling system.
            </p>
            <div class="mt-10">
                <button id="launch-app-btn"
                    class="inline-block px-10 py-4 text-xl font-bold text-white rounded-lg shadow-xl btn-gradient-sky hover:shadow-2xl transform hover:-translate-y-1 transition duration-300 focus:outline-none focus:ring-4 focus:ring-sky-400 focus:ring-offset-2">
                    Launch Scheduler App
                </button>
                <p class="mt-3 text-sm text-gray-500">(No login required in local mode)</p>
            </div>
        </header>

        <main class="w-full max-w-5xl mt-10">
            <section class="mb-16">
                <h2 class="text-3xl font-bold text-center mb-8 border-b-2 border-sky-200 pb-3">Key Features</h2>
                <div class="grid md:grid-cols-3 gap-8 text-center">
                    <div class="bg-white p-6 rounded-2xl shadow-lg border border-gray-100">
                        <h3 class="text-xl font-bold text-sky-700 mb-2">Advanced Scheduling</h3>
                        <p class="text-gray-600">Set start/end dates and choose between specific days or a repeating N-day frequency.</p>
                    </div>
                    <div class="bg-white p-6 rounded-2xl shadow-lg border border-gray-100">
                        <h3 class="text-xl font-bold text-sky-700 mb-2">Hardware Sync</h3>
                        <p class="text-gray-600">The interface syncs directly with the ESP32 hardware, providing real-time dispensing status.</p>
                    </div>
                    <div class="bg-white p-6 rounded-2xl shadow-lg border border-gray-100">
                        <h3 class="2xl font-bold text-sky-700 mb-2">Local & Private</h3>
                        <p class="text-gray-600">All your schedule data is stored locally in your browser. No cloud, no accounts, complete privacy.</p>
                    </div>
                </div>
            </section>
            <section>
                <h2 class="3xl font-bold text-center mb-8 border-b-2 border-sky-200 pb-3">Frequently Asked Questions</h2>
                <div class="space-y-6 max-w-3xl mx-auto">
                    <div class="bg-white p-5 rounded-xl shadow-md">
                        <h4 class="font-semibold text-lg mb-1">How does the "Sync to Dispenser" button work?</h4>
                        <p class="text-gray-600">When you click "Sync," the application sends your saved schedules over your local Wi-Fi to the hardware pill dispenser. The dispenser must be powered on and connected for the sync to be successful.</p>
                    </div>
                     <div class="bg-white p-5 rounded-xl shadow-md">
                        <h4 class="font-semibold text-lg mb-1">What does the "Dose Taken" button do?</h4>
                        <p class="text-gray-600">After the dispenser confirms a pill is ready, the "Dose Taken" button will appear. Clicking it tells the hardware that you have picked up the medication, which turns off the alarm and resets the system for the next dose.</p>
                    </div>
                    <div class="bg-white p-5 rounded-xl shadow-md">
                        <h4 class="font-semibold text-lg mb-1">What happens if I clear my browser cache?</h4>
                        <p class="gray-600">Clearing your browser's cache or site data will permanently delete all saved schedules from the web interface. Be sure to sync your schedules to the physical dispenser regularly.</p>
                    </div>
                </div>
            </section>
        </main>

        <footer class="w-full max-w-5xl text-center py-10 mt-16">
            <p class="gray-500">&copy; 2025 Smart Pill Dispenser Project. All Rights Reserved.</p>
        </footer>
    </div>

    <div id="app" class="min-h-screen p-4 sm:p-8 flex-col items-center hidden">
        <header class="w-full max-w-5xl text-center mb-10 pt-4">
            <div class="flex justify-center items-center mb-2">
                <h1 class="text-5xl font-extrabold text-gray-900 tracking-tight">Pill Dispenser Schedule</h1>
            </div>
            <p class="text-xl text-gray-600 border-b border-gray-200 pb-4">Smart medication management for your health (Local Mode).</p>

            <div class="mt-4 space-x-4">
                <button id="go-back-btn" class="px-6 py-3 text-lg font-bold text-gray-700 bg-gray-200 rounded-lg shadow-lg hover:bg-gray-300 transition duration-200 focus:outline-none focus:ring-4 focus:ring-gray-400 focus:ring-offset-2">
                    &larr; Back to Main Page
                </button>
                <button id="sync-button" class="px-6 py-3 text-lg font-bold text-white rounded-lg shadow-xl btn-gradient-teal hover:shadow-2xl transition duration-200 focus:outline-none focus:ring-4 focus:ring-teal-400 focus:ring-offset-2">
                    Sync Schedules to Dispenser
                </button>
                
                <button id="test-motor-1-btn" class="px-4 py-3 text-lg font-bold text-white bg-orange-500 rounded-lg shadow-lg hover:bg-orange-600 transition duration-200">
                    Test Motor 1
                </button>
                <button id="test-motor-2-btn" class="px-4 py-3 text-lg font-bold text-white bg-orange-500 rounded-lg shadow-lg hover:bg-orange-600 transition duration-200">
                    Test Motor 2
                </button>
                </div>
            <p id="sync-message" class="mt-2 text-sm hidden"></p>
        </header>

        <div id="reminder-alert" class="w-full max-w-5xl p-5 mb-8 text-center rounded-2xl shadow-2xl transition-all duration-500 ease-in-out hidden flex items-center justify-between" role="alert">
            <p id="reminder-text" class="2xl font-bold flex-grow text-left">No active pill reminders right now.</p>
            <button id="dismiss-reminder-btn" class="ml-4 px-4 py-2 bg-green-500 hover:bg-green-600 text-white font-semibold rounded-lg shadow-md transition duration-150 hidden">
                Dose Taken
            </button>
        </div>

        <main class="w-full max-w-5xl grid grid-cols-1 lg:grid-cols-3 gap-8">
            <div id="form-container" class="lg:col-span-1 bg-white p-8 rounded-3xl shadow-2xl h-fit border border-sky-100/50 transition hover:shadow-3xl relative">
                <h2 id="form-title" class="2xl font-bold mb-6 text-sky-700 border-b-2 border-sky-200 pb-3">New Schedule</h2>
                <form id="schedule-form">
                    <div class="space-y-4 mb-6 p-4 bg-gray-50 rounded-xl border border-gray-200">
                        <h3 class="text-lg font-semibold text-gray-800">Medication Details</h3>
                        <div>
                            <label for="pill-name" class="block text-sm font-semibold text-gray-700 mb-1">Pill Name</label>
                            <input type="text" id="pill-name" required placeholder="e.g., Vitamin D, Ibuprofen"
                                   class="w-full px-4 py-2 border border-gray-300 rounded-lg shadow-inner focus:ring-sky-500">
                        </div>
                        <div>
                            <label for="pill-dosage" class="block text-sm font-semibold text-gray-700 mb-1">Pill Dosage</label>
                            <input type="text" id="pill-dosage" required placeholder="e.g., 500 mg, 1 tablet"
                                   class="w-full px-4 py-2 border border-gray-300 rounded-lg shadow-inner focus:ring-sky-500">
                        </div>
                        <div class="pt-2">
                            <label for="compartment-num" class="block text-sm font-semibold text-gray-700 mb-1">Compartment Number (1-N)</label>
                            <input type="number" id="compartment-num" required min="1" max="10" placeholder="e.g., 1 or 3"
                                   class="w-full px-4 py-2 border border-gray-300 rounded-lg shadow-inner focus:ring-teal-500">
                        </div>
                    </div>

                    <div class="space-y-4 mb-6 p-4 bg-sky-50 rounded-xl border border-sky-200">
                        <h3 class="text-lg font-semibold text-sky-800">Timing & Frequency</h3>
                        <div>
                            <label for="start-date" class="block text-sm font-semibold text-gray-700 mb-1">Start Date</label>
                            <input type="date" id="start-date" required
                                   class="w-full px-4 py-2 border border-sky-300 rounded-lg shadow-sm focus:ring-sky-500">
                        </div>
                        <div>
                            <label for="end-date" class="block text-sm font-semibold text-gray-700 mb-1">End Date</label>
                            <input type="date" id="end-date" required
                                   class="w-full px-4 py-2 border border-sky-300 rounded-lg shadow-sm focus:ring-sky-500">
                        </div>
                        <div class="pt-2 border-t border-sky-200">
                            <label for="time-of-day-1" class="block text-sm font-semibold text-gray-700 mb-1">Time of Day (1st Dose)</label>
                            <input type="time" id="time-of-day-1" required
                                   class="time-input w-full px-4 py-2 border border-sky-300 rounded-lg shadow-sm focus:ring-sky-500">
                        </div>
                        <div class="pt-2">
                            <input type="checkbox" id="multiple-times-checkbox" class="h-4 w-4 text-sky-600 border-sky-300 rounded focus:ring-sky-500">
                            <label for="multiple-times-checkbox" class="ml-2 text-sm font-semibold text-gray-700">Schedule multiple times?</label>
                        </div>
                        <div id="additional-times-section" class="space-y-4 hidden">
                            <button type="button" id="add-time-btn"
                                    class="w-full py-2 px-4 bg-sky-100 text-sky-600 font-semibold rounded-lg shadow-md hover:bg-sky-200 border border-sky-300">
                                + Add Another Dose Time
                            </button>
                            <div id="additional-times-container" class="space-y-3"></div>
                        </div>
                    </div>

                    <div class="mb-6 p-4 bg-gray-50 rounded-xl border border-gray-200">
                        <label class="block text-sm font-semibold text-gray-700 mb-2">Scheduling Mode</label>
                        <div class="flex space-x-4 mb-4">
                            <label class="flex items-center"><input type="radio" name="scheduling-mode" value="days" id="mode-days" checked class="h-4 w-4 text-teal-600 focus:ring-teal-500"><span class="ml-2 text-sm font-medium">Specific Days</span></label>
                            <label class="flex items-center"><input type="radio" name="scheduling-mode" value="frequency" id="mode-frequency" class="h-4 w-4 text-teal-600 focus:ring-teal-500"><span class="ml-2 text-sm font-medium">Repeating Frequency</span></label>
                        </div>
                        <div id="days-of-week-group">
                            <label class="block text-sm font-semibold text-gray-700 mb-2">Days of Week</label>
                            <div id="days-of-week" class="grid grid-cols-4 sm:grid-cols-7 gap-2 text-sm"></div>
                        </div>
                        <div id="frequency-input-group" class="hidden space-y-2 p-3 mt-4 bg-white rounded-lg border border-sky-300">
                            <label for="frequency-days" class="block text-sm font-semibold text-gray-700">Repeat Every N Days</label>
                            <input type="number" id="frequency-days" min="1" max="365" placeholder="Enter '1' for daily"
                                   class="w-full px-4 py-2 border border-gray-300 rounded-lg shadow-inner focus:ring-sky-500">
                            <p classt="text-xs text-gray-500">e.g., 1 for Daily, 7 for Weekly.</p>
                        </div>
                    </div>

                    <button type="button" id="cancel-edit-btn" class="w-full py-3 px-4 bg-gray-300 text-gray-700 font-semibold rounded-lg shadow-md hover:bg-gray-400 mb-2 hidden">
                        Cancel Edit
                    </button>
                    <button type="submit" id="save-button" class="w-full py-3 px-4 text-white font-semibold rounded-lg shadow-xl btn-gradient-sky hover:shadow-2xl">
                        Save New Schedule
                    </button>
                    <p id="form-message" class="mt-3 text-center text-sm hidden"></p>
                </form>
            </div>

            <div class="lg:col-span-2 bg-white p-8 rounded-3xl shadow-2xl border border-gray-100">
                <h2 class="text-2xl font-bold mb-6 text-gray-800 border-b-2 border-gray-200 pb-3">Active Schedules</h2>
                <div id="schedules-list" class="space-y-4"></div>
                <p id="error-display" class="mt-4 text-red-600 hidden"></p>
            </div>
        </main>
    </div>

    <script>
        // --- PAGE SWITCHER LOGIC ---
        const landingPageEl = document.getElementById('landing-page');
        const appEl = document.getElementById('app');
        const launchBtnEl = document.getElementById('launch-app-btn');
        const goBackBtnEl = document.getElementById('go-back-btn');

        launchBtnEl.addEventListener('click', () => {
            landingPageEl.classList.add('hidden');
            appEl.classList.remove('hidden');
            appEl.classList.add('flex');
        });
        goBackBtnEl.addEventListener('click', () => {
            appEl.classList.add('hidden');
            appEl.classList.remove('flex');
            landingPageEl.classList.remove('hidden');
        });

        // --- SCHEDULER APP LOGIC ---
        const LOCAL_STORAGE_KEY = 'pill_schedules_local_v2';
        const DAYS = ['Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat'];
        
        // --- MODIFIED: Use relative paths instead of a hardcoded IP ---
        const DISPENSER_API_URL = '/schedules';
        const STATUS_API_URL = '/status';
        const DISPENSE_ACK_API_URL = '/dispense';
        
        // NEW TEST ENDPOINTS
        const TEST_MOTOR_1_API_URL = '/test/motor1';
        const TEST_MOTOR_2_API_URL = '/test/motor2';


        let editingScheduleId = null;
        let activeSchedules = [];
        let pendingDeleteId = null;
        let currentReminderPill = null;

        const formEl = document.getElementById('schedule-form');
        const formMessageEl = document.getElementById('form-message');
        const schedulesListEl = document.getElementById('schedules-list');
        const daysContainerEl = document.getElementById('days-of-week');
        const daysGroupEl = document.getElementById('days-of-week-group');
        const frequencyInputGroupEl = document.getElementById('frequency-input-group');
        const modeDaysEl = document.getElementById('mode-days');
        const modeFrequencyEl = document.getElementById('mode-frequency');
        const formTitleEl = document.getElementById('form-title');
        const saveButtonEl = document.getElementById('save-button');
        const cancelEditButtonEl = document.getElementById('cancel-edit-btn');
        const reminderAlertEl = document.getElementById('reminder-alert');
        const reminderTextEl = document.getElementById('reminder-text');
        const dismissReminderBtn = document.getElementById('dismiss-reminder-btn');
        const syncButtonEl = document.getElementById('sync-button');
        const syncMessageEl = document.getElementById('sync-message');
        const pillNameEl = document.getElementById('pill-name');
        const pillDosageEl = document.getElementById('pill-dosage');
        const compartmentNumEl = document.getElementById('compartment-num');
        const startDateEl = document.getElementById('start-date');
        const endDateEl = document.getElementById('end-date');
        const frequencyDaysEl = document.getElementById('frequency-days');
        const timeOfDay1El = document.getElementById('time-of-day-1');
        const multipleTimesCheckboxEl = document.getElementById('multiple-times-checkbox');
        const additionalTimesSectionEl = document.getElementById('additional-times-section');
        const additionalTimesContainerEl = document.getElementById('additional-times-container');
        const addTimeBtnEl = document.getElementById('add-time-btn');
        // NEW TEST BUTTON REFERENCES
        const testMotor1Btn = document.getElementById('test-motor-1-btn');
        const testMotor2Btn = document.getElementById('test-motor-2-btn');


        function showMessage(message, type = 'success') {
            formMessageEl.textContent = message;
            formMessageEl.className = `mt-3 text-center text-sm p-2 rounded ${type === 'success' ? 'text-green-800 bg-green-100' : 'text-red-800 bg-red-100'}`;
            formMessageEl.classList.remove('hidden');
            setTimeout(() => { formMessageEl.classList.add('hidden'); }, 3000);
        }

        function displaySyncMessage(message, type) {
            syncMessageEl.textContent = message;
            syncMessageEl.className = `mt-2 text-center text-sm font-semibold p-2 rounded ${type === 'success' ? 'text-green-800 bg-green-200' : 'text-red-800 bg-red-200'} w-full max-w-lg mx-auto`;
            syncMessageEl.classList.remove('hidden');
            setTimeout(() => { syncMessageEl.classList.add('hidden'); }, 5000);
        }

        function createDayCheckboxes(selectedDays = []) {
            daysContainerEl.innerHTML = DAYS.map(day => `
                <label class="day-label">
                    <input type="checkbox" name="day" value="${day}" ${selectedDays.includes(day) ? 'checked' : ''} class="day-checkbox">
                    <div class="day-box"><span class="font-semibold">${day}</span></div>
                </label>`).join('');
        }

        function getOrdinal(n) { const s = ["th", "st", "nd", "rd"], v = n % 100; return n + (s[(v - 20) % 10] || s[v] || s[0]); }

        function addTimeInput(timeValue = null) {
            const count = document.querySelectorAll('.time-input').length;
            if (count >= 4) { showMessage("Maximum of 5 dose times supported.", "error"); return; }
            const newTimeGroup = document.createElement('div');
            newTimeGroup.className = 'time-input-group flex items-end space-x-2';
            newTimeGroup.innerHTML = `
                <div class="flex-grow">
                    <label class="block text-sm font-medium text-gray-700 mb-1">${getOrdinal(count + 2)} Dose Time</label>
                    <input type="time" required class="time-input w-full px-4 py-2 border border-gray-300 rounded-lg" value="${timeValue || ''}">
                </div>
                <button type="button" class="remove-time-btn p-2 h-[42px] w-10 bg-red-100 text-red-600 rounded-lg hover:bg-red-200">
                    <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5 mx-auto" viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M10 18a8 8 0 100-16 8 8 0 000 16zM7 9a1 1 0 000 2h6a1 1 0 100-2H7z" clip-rule="evenodd" /></svg>
                </button>`;
            newTimeGroup.querySelector('.remove-time-btn').addEventListener('click', () => newTimeGroup.remove());
            additionalTimesContainerEl.appendChild(newTimeGroup);
        }

        function toggleAdditionalTimes(checked, times = []) {
            additionalTimesContainerEl.innerHTML = '';
            additionalTimesSectionEl.classList.toggle('hidden', !checked);
            if (checked && times.length > 1) {
                times.slice(1).forEach(time => addTimeInput(time));
            }
        }

        function toggleDaysAndFrequency() {
            const isFrequency = modeFrequencyEl.checked;
            daysGroupEl.classList.toggle('opacity-50', isFrequency);
            daysGroupEl.classList.toggle('pointer-events-none', isFrequency);
            frequencyInputGroupEl.classList.toggle('hidden', !isFrequency);
            if (!isFrequency) frequencyDaysEl.value = '';
        }

        function resetForm() {
            formEl.reset();
            formTitleEl.textContent = 'New Schedule';
            saveButtonEl.textContent = 'Save New Schedule';
            saveButtonEl.className = saveButtonEl.className.replace('btn-gradient-teal', 'btn-gradient-sky');
            cancelEditButtonEl.classList.add('hidden');
            editingScheduleId = null;
            pendingDeleteId = null;
            toggleAdditionalTimes(false);
            createDayCheckboxes([]);
            modeDaysEl.checked = true;
            toggleDaysAndFrequency();
        }

        function loadSchedules() { try { const d = localStorage.getItem(LOCAL_STORAGE_KEY); return d ? JSON.parse(d) : []; } catch (e) { return []; } }
        function saveSchedules(s) {
            try {
                localStorage.setItem(LOCAL_STORAGE_KEY, JSON.stringify(s));
                activeSchedules = s;
                renderSchedules(s);
            } catch (e) {
                showMessage('Error saving schedules locally.', 'error');
            }
        }

        function renderSchedules(schedules) {
            if (schedules.length === 0) {
                schedulesListEl.innerHTML = '<p class="text-center py-8 text-gray-500 bg-gray-50 rounded-lg border border-dashed">No active schedules. Add one!</p>';
                return;
            }
            const sorted = schedules.sort((a, b) => a.compartmentNum - b.compartmentNum || a.times[0].localeCompare(b.times[0]));
            pendingDeleteId = null;
            schedulesListEl.innerHTML = sorted.map(s => {
                const scheduleText = s.mode === 'frequency'
                    ? `<span class="font-bold text-pink-600">Every ${s.frequencyDays} day(s)</span>`
                    : (s.days.length === 7 ? '<span class="font-bold text-teal-700">Daily</span>' : s.days.join(', '));
                const durationText = `From ${s.startDate} to ${s.endDate}`;
                const times = s.times.join(' • ');
                const timeDisp = s.times.length > 1 ? `<p class="text-lg font-mono">${times}</p>` : `<p class="text-3xl font-extrabold text-sky-700">${times}</p>`;
                return `<div class="flex flex-col sm:flex-row items-start p-5 bg-white rounded-xl shadow-lg border-l-4 border-sky-400">
                    <div class="flex-grow">
                        <h3 class="2xl font-bold">${s.pillName}</h3>
                        <p class="text-sm font-semibold text-gray-600 mb-1">Dose: <span class="text-teal-600 font-bold">${s.dosage}</span> | Comp: <span class="text-teal-600 font-bold">${s.compartmentNum}</span></p>
                        ${timeDisp}
                        <p class="text-sm text-gray-600">
                            <span class="font-semibold">Schedule:</span> ${scheduleText}
                            <span class="text-xs italic ml-2">${durationText}</span>
                        </p>
                    </div>
                    <div class="flex space-x-2 flex-shrink-0 mt-3 sm:mt-0">
                        <button data-id="${s.id}" class="edit-btn p-3 bg-teal-100 text-teal-700 rounded-lg hover:bg-teal-200" title="Edit"><svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor"><path d="M13.586 3.586a2 2 0 112.828 2.828l-.793.793-2.828-2.828.793-.793zm-1.828 4.314l-4.95 4.95a1 1 0 01-.39.242l-2.426.809c-.273.091-.555.087-.824-.017s-.485-.306-.599-.588l-.809-2.426a1 1 0 01.242-.39l4.95-4.95 2.828 2.828z" /></svg></button>
                        <button data-id="${s.id}" class="delete-btn p-3 bg-red-100 text-red-700 rounded-lg hover:bg-red-200" title="Delete"><svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5" viewBox="0 0 20 20" fill="currentColor"><path fill-rule="evenodd" d="M9 2a1 1 0 00-.894.553L7.382 4H4a1 1 0 000 2v10a2 2 0 002 2h8a2 2 0 002-2V6a1 1 0 100-2h-3.382l-.728-1.447A1 1 0 0011 2H9zM7 8a1 1 0 012 0v6a1 1 0 11-2 0V8zm5-1a1 1 0 00-1 1v6a1 1 0 102 0V8a1 1 0 00-1-1z" clip-rule="evenodd" /></svg></button>
                    </div></div>`;
            }).join('');

            document.querySelectorAll('.edit-btn').forEach(b => b.addEventListener('click', e => editSchedule(e.currentTarget.getAttribute('data-id'))));
            document.querySelectorAll('.delete-btn').forEach(b => {
                const id = b.getAttribute('data-id');
                const icon = b.innerHTML;
                b.addEventListener('click', () => {
                    if (pendingDeleteId === id) {
                        deleteSchedule(id);
                    } else {
                        document.querySelectorAll('.delete-btn').forEach(btn => {
                            btn.innerHTML = icon;
                            btn.classList.remove('bg-yellow-200', 'text-yellow-800');
                        });
                        pendingDeleteId = id;
                        b.innerHTML = '<span class="text-xs font-bold">CONFIRM?</span>';
                        b.classList.add('bg-yellow-200', 'text-yellow-800');
                        setTimeout(() => {
                            if (pendingDeleteId === id) {
                                pendingDeleteId = null;
                                b.innerHTML = icon;
                                b.classList.remove('bg-yellow-200', 'text-yellow-800');
                            }
                        }, 3000);
                    }
                });
            });
        }
        
        function editSchedule(id) {
            const s = activeSchedules.find(sch => sch.id === id);
            if (!s) return;
            resetForm();
            editingScheduleId = id;
            formTitleEl.textContent = 'Edit Schedule';
            saveButtonEl.textContent = 'Update Schedule';
            saveButtonEl.className = saveButtonEl.className.replace('btn-gradient-sky', 'btn-gradient-teal');
            cancelEditButtonEl.classList.remove('hidden');

            pillNameEl.value = s.pillName;
            pillDosageEl.value = s.dosage;
            compartmentNumEl.value = s.compartmentNum;
            startDateEl.value = s.startDate;
            endDateEl.value = s.endDate;
            timeOfDay1El.value = s.times[0];
            toggleAdditionalTimes(s.times.length > 1, s.times);
            multipleTimesCheckboxEl.checked = s.times.length > 1;

            if (s.mode === 'frequency') {
                modeFrequencyEl.checked = true;
                frequencyDaysEl.value = s.frequencyDays;
            } else {
                modeDaysEl.checked = true;
                createDayCheckboxes(s.days);
            }
            toggleDaysAndFrequency();
            formContainer.scrollIntoView({ behavior: 'smooth' });
        }

        function deleteSchedule(id) {
            const updated = activeSchedules.filter(s => s.id !== id);
            saveSchedules(updated);
            showMessage('Schedule deleted.', 'success');
        }

        function saveSchedule(e) {
            e.preventDefault();
            const id = editingScheduleId || `sch_${Date.now()}`;
            const mode = modeFrequencyEl.checked ? 'frequency' : 'days';
            const times = Array.from(document.querySelectorAll('.time-input')).map(i => i.value).filter(Boolean);
            let days = [], frequency = null;

            if (mode === 'days') {
                days = Array.from(document.querySelectorAll('[name="day"]:checked')).map(cb => cb.value);
                if (days.length === 0) return showMessage('Please select at least one day.', 'error');
            } else {
                frequency = parseInt(frequencyDaysEl.value);
                if (!frequency || frequency < 1) return showMessage('Please enter a valid frequency.', 'error');
            }
            if (new Date(startDateEl.value) > new Date(endDateEl.value)) return showMessage('Start date cannot be after end date.', 'error');

            const schedule = {
                id, pillName: pillNameEl.value, dosage: pillDosageEl.value, compartmentNum: compartmentNumEl.value,
                startDate: startDateEl.value, endDate: endDateEl.value, mode, days, frequencyDays: frequency, times
            };
            const otherSchedules = activeSchedules.filter(s => s.id !== id);
            saveSchedules([...otherSchedules, schedule]);
            showMessage(`Schedule ${editingScheduleId ? 'updated' : 'saved'}!`);
            resetForm();
        }

        async function syncToDispenser() {
            displaySyncMessage('Syncing...', 'success');
            try {
                const res = await fetch(DISPENSER_API_URL, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(activeSchedules),
                    signal: AbortSignal.timeout(5000)
                });
                if (!res.ok) throw new Error(`Dispenser responded with status ${res.status}`);
                displaySyncMessage('Sync Successful!', 'success');
            } catch (err) {
                displaySyncMessage(`Sync failed: ${err.message}. Check connection.`, 'error');
            }
        }

        // NEW: Function to send manual test command
        async function testMotor(motorNum) {
             const url = motorNum === 1 ? TEST_MOTOR_1_API_URL : TEST_MOTOR_2_API_URL;
             syncMessageEl.classList.add('hidden'); // Clear prior sync messages
             const testBtn = motorNum === 1 ? testMotor1Btn : testMotor2Btn;
             const originalText = testBtn.textContent;
             testBtn.textContent = 'Running...';
             testBtn.disabled = true;

             try {
                 // INCREASED TIMEOUT: From 3000ms to 8000ms (8 seconds)
                 const res = await fetch(url, { method: 'POST', signal: AbortSignal.timeout(8000) }); 
                 
                 if (res.ok) {
                     displaySyncMessage(`Motor ${motorNum} cycle initiated successfully!`, 'success');
                 } else if (res.status === 409) {
                     displaySyncMessage(`Motor ${motorNum} busy: Wait for dispensing to finish.`, 'error');
                 } else {
                     throw new Error(`Server status ${res.status}`);
                 }
             } catch (err) {
                 // Check if the error is due to an abort (timeout)
                 if (err.name === 'AbortError' || err.message.includes('timeout')) {
                     displaySyncMessage(`Test failed: Operation timed out. Increase C++ DISPENSE_TIME_MS or check wiring.`, 'error');
                 } else {
                     displaySyncMessage(`Test failed for Motor ${motorNum}: ${err.message}`, 'error');
                 }
             } finally {
                 testBtn.textContent = originalText;
                 testBtn.disabled = false;
             }
        }
        
        async function acknowledgeDispense() {
            if (!currentReminderPill) return;
            dismissReminderBtn.disabled = true;
            try {
                const res = await fetch(DISPENSE_ACK_API_URL, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify({ compartment: currentReminderPill.compartment }),
                    signal: AbortSignal.timeout(3000)
                });
                if (!res.ok) throw new Error('Acknowledgement failed');
                currentReminderPill = null;
                updateReminderAlert();
            } catch (e) {
                showMessage('Failed to send acknowledgement.', 'error');
            } finally {
                dismissReminderBtn.disabled = false;
            }
        }
        
        function updateReminderAlert() {
             if (!currentReminderPill) {
                reminderAlertEl.classList.add('hidden');
                return;
            }
            reminderAlertEl.classList.remove('hidden');
            const { pill, compartment, status } = currentReminderPill;
            let msg = '', cls = '', showDismiss = false;

            switch(status) {
                case 'dispensing':
                    msg = `⚙ DISPENSING: Preparing ${pill} (Comp: ${compartment})...`;
                    cls = 'bg-teal-100 text-teal-800 border-teal-400';
                    break;
                case 'confirmed':
                    msg = `✅ DISPENSED! ${pill} (Comp: ${compartment}) is ready for pickup.`;
                    cls = 'bg-green-100 text-green-800 border-green-500';
                    showDismiss = true;
                    break;
                default: // due_now or overdue
                    msg = `🚨 ${status === 'due_now' ? 'DUE NOW' : 'MISSED DOSE'}: Take ${pill} (Comp: ${compartment}).`;
                    cls = `bg-${status === 'due_now' ? 'yellow' : 'red'}-100 text-${status === 'due_now' ? 'yellow' : 'red'}-900 border-${status === 'due_now' ? 'yellow' : 'red'}-500`;
                    reminderAlertEl.classList.toggle('alert-pulse', status === 'due_now');
            }
            reminderTextEl.textContent = msg;
            reminderAlertEl.className = `w-full max-w-5xl p-5 mb-8 rounded-2xl shadow-2xl flex items-center justify-between ${cls}`;
            dismissReminderBtn.classList.toggle('hidden', !showDismiss);
        }

        function isScheduledToday(now, schedule) {
            const today = new Date(now.getFullYear(), now.getMonth(), now.getDate());
            const start = new Date(schedule.startDate);
            const end = new Date(schedule.endDate);
            if (today < start || today > end) return false;

            if (schedule.mode === 'days') {
                return schedule.days.includes(DAYS[now.getDay()]);
            }
            if (schedule.mode === 'frequency') {
                const diffTime = Math.abs(today - start);
                const diffDays = Math.ceil(diffTime / (1000 * 60 * 60 * 24));
                return diffDays % schedule.frequencyDays === 0;
            }
            return false;
        }

        function checkLocalReminders() {
            const now = new Date();
            const nowTime = `${String(now.getHours()).padStart(2, '0')}:${String(now.getMinutes()).padStart(2, '0')}`;
            
            const dueReminders = [];
            for (const s of activeSchedules) {
                if (isScheduledToday(now, s)) {
                    for (const t of s.times) {
                        const h1 = parseInt(t.split(':')[0]), m1 = parseInt(t.split(':')[1]);
                        const h2 = now.getHours(), m2 = now.getMinutes();
                        const diff = (h1 * 60 + m1) - (h2 * 60 + m2);
                        if (diff >= -60 && diff <= 5) {
                             dueReminders.push({ pill: s.pillName, compartment: s.compartmentNum, time: t, status: diff >= 0 ? 'due_now' : 'overdue' });
                        }
                    }
                }
            }
            if (!currentReminderPill || !['dispensing', 'confirmed'].includes(currentReminderPill.status)) {
                currentReminderPill = dueReminders.length > 0 ? dueReminders[0] : null;
            }
        }
        
        async function pollDispenserStatus() {
            try {
                const res = await fetch(STATUS_API_URL, { signal: AbortSignal.timeout(2000) });
                if (!res.ok) return; 
                const status = await res.json();
                if (status.isDispensing) {
                    currentReminderPill = { pill: status.pill, compartment: status.compartment, status: 'dispensing' };
                } else if (status.dispensedConfirmed) {
                    currentReminderPill = { pill: status.pill, compartment: status.compartment, status: 'confirmed' };
                } else {
                    checkLocalReminders();
                }
            } catch (e) {
                 checkLocalReminders();
            }
            updateReminderAlert();
        }

        // --- App Initialization ---
        window.onload = () => {
            createDayCheckboxes();
            modeDaysEl.addEventListener('change', toggleDaysAndFrequency);
            modeFrequencyEl.addEventListener('change', toggleDaysAndFrequency);
            formEl.addEventListener('submit', saveSchedule);
            multipleTimesCheckboxEl.addEventListener('change', e => toggleAdditionalTimes(e.target.checked));
            addTimeBtnEl.addEventListener('click', () => addTimeInput());
            cancelEditButtonEl.addEventListener('click', resetForm);
            syncButtonEl.addEventListener('click', syncToDispenser);
            dismissReminderBtn.addEventListener('click', acknowledgeDispense);
            
            // ATTACH NEW TEST BUTTON LISTENERS
            testMotor1Btn.addEventListener('click', () => testMotor(1));
            testMotor2Btn.addEventListener('click', () => testMotor(2));


            activeSchedules = loadSchedules();
            renderSchedules(activeSchedules);
            pollDispenserStatus();
            setInterval(pollDispenserStatus, 5000); 
        };
    </script>
</body>
</html>
)rawliteral";

#endif // WEBPAGE_H_INCLUDED 